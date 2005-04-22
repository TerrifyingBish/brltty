/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2005 by The BRLTTY Team. All rights reserved.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation.  Please see the file COPYING for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

/* brltest.c - Test progrm for the Braille display library
 * $Id: brltest.c,v 1.3 1996/09/24 01:04:24 nn201 Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include "options.h"
#include "brl.h"
#include "tbl.h"
#include "misc.h"
#include "message.h"
#include "scr.h"
#include "defaults.h"

int updateInterval = DEFAULT_UPDATE_INTERVAL;
static BrailleDisplay brl;

static char *opt_brailleDevice;
static char *opt_libraryDirectory;
static char *opt_dataDirectory;

BEGIN_OPTION_TABLE
  {"device", "device", 'd', 0, 0,
   &opt_brailleDevice, BRAILLE_DEVICE,
   "Path to device for accessing braille display."},

  {"data-directory", "directory", 'D', 0, 0,
   &opt_dataDirectory, DATA_DIRECTORY,
   "Path to directory for driver help and configuration files."},

  {"library-directory", "directory", 'L', 0, 0,
   &opt_libraryDirectory, LIBRARY_DIRECTORY,
   "Path to directory for loading drivers."},
END_OPTION_TABLE

void
message (const char *string, short flags) {
  int length = strlen(string);
  int limit = brl.x * brl.y;

  clearStatusCells(&brl);

  memset(brl.buffer, ' ', brl.x*brl.y);
  while (length) {
    int count = (length <= limit)? length: (limit - 1);
    int index;
    for (index=0; index<count; brl.buffer[index++]=*string++);
    if (length -= count)
      brl.buffer[(brl.x * brl.y) - 1] = '-';
    else
      while (index < limit) brl.buffer[index++] = ' ';
    writeBrailleBuffer(&brl);

    if (length) {
      int timer = 0;
      while (braille->readCommand(&brl, BRL_CTX_MESSAGE) == EOF) {
        if (timer > 4000) break;
        approximateDelay(updateInterval);
        timer += updateInterval;
      }
    }
  }
}

int
main (int argc, char *argv[]) {
  int status;
  const char *driver = NULL;
  void *object;

  processOptions(optionTable, optionCount,
                 "brltest", &argc, &argv,
                 NULL, NULL, NULL,
                 "[driver [parameter=value ...]]");

  if (argc) {
    driver = *argv++;
    --argc;
  }
  if (!opt_brailleDevice) opt_brailleDevice = BRAILLE_DEVICE;

  if ((braille = loadBrailleDriver(driver, &object, opt_libraryDirectory))) {
    const char *const *parameterNames = braille->parameters;
    char **parameterSettings;
    if (!parameterNames) {
      static const char *const noNames[] = {NULL};
      parameterNames = noNames;
    }
    {
      const char *const *name = parameterNames;
      unsigned int count;
      char **setting;
      while (*name) ++name;
      count = name - parameterNames;
      if (!(parameterSettings = malloc((count + 1) * sizeof(*parameterSettings)))) {
        fprintf(stderr, "%s: Insufficient memory.\n", programName);
        exit(9);
      }
      setting = parameterSettings;
      while (count--) *setting++ = "";
      *setting = NULL;
    }
    while (argc) {
      char *assignment = *argv++;
      int ok = 0;
      char *delimiter = strchr(assignment, '=');
      if (!delimiter) {
        LogPrint(LOG_ERR, "Missing braille driver parameter value: %s", assignment);
      } else if (delimiter == assignment) {
        LogPrint(LOG_ERR, "Missing braille driver parameter name: %s", assignment);
      } else {
        size_t nameLength = delimiter - assignment;
        const char *const *name = parameterNames;
        while (*name) {
          if (strncasecmp(assignment, *name, nameLength) == 0) {
            parameterSettings[name - parameterNames] = delimiter + 1;
            ok = 1;
            break;
          }
          ++name;
        }
        if (!ok) LogPrint(LOG_ERR, "Invalid braille driver parameter: %s", assignment);
      }
      if (!ok) exit(2);
      --argc;
    }

    if (chdir(opt_dataDirectory) != -1) {
      makeUntextTable();
      initializeBrailleDisplay(&brl);
      brl.dataDirectory = opt_dataDirectory;
      braille->identify();		/* start-up messages */
      if (braille->open(&brl, parameterSettings, opt_brailleDevice)) {
        if (allocateBrailleBuffer(&brl)) {
#ifdef ENABLE_LEARN_MODE
          learnMode(&brl, updateInterval, 10000);
#else /* ENABLE_LEARN_MODE */
          message("braille test", 0);
#endif /* ENABLE_LEARN_MODE */
          braille->close(&brl);		/* finish with the display */
          status = 0;
        } else {
          LogPrint(LOG_ERR, "Can't allocate braille buffer.");
          status = 6;
        }
      } else {
        LogPrint(LOG_ERR, "Can't initialize braille driver.");
        status = 5;
      }
    } else {
      LogPrint(LOG_ERR, "Can't change directory to '%s': %s",
               opt_dataDirectory, strerror(errno));
      status = 4;
    }
  } else {
    LogPrint(LOG_ERR, "Can't load braille driver.");
    status = 3;
  }
  return status;
}

/* dummy function to allow brl.o to link... */
void
setHelpPageNumber (short page) {
}
int
insertString (const char *string) {
  return 0;
}
