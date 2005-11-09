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

#include "prologue.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "options.h"
#include "misc.h"
#include "brl.h"
#include "tbl.h"

static char *opt_inputFormat;
static char *opt_outputFormat;
static int opt_translate;
static char *opt_dataDirectory;

BEGIN_OPTION_TABLE
  {"input-format", "format", 'i', 0, 0,
   &opt_inputFormat, NULL,
   "Format of input file."},

  {"output-format", "format", 'o', 0, 0,
   &opt_outputFormat, NULL,
   "Format of output file."},

  {"translate", NULL, 't', 0, 0,
   &opt_translate, NULL,
   "Translate."},

  {"data-directory", "file", 'D', 0, OPT_Hidden,
   &opt_dataDirectory, DATA_DIRECTORY,
   "Path to directory for configuration files."},
END_OPTION_TABLE

static const DotsTable dotsInternal = {
  BRL_DOT1, BRL_DOT2, BRL_DOT3, BRL_DOT4,
  BRL_DOT5, BRL_DOT6, BRL_DOT7, BRL_DOT8
};

static const DotsTable dots12345678 = {
  0X01, 0X02, 0X04, 0X08, 0X10, 0X20, 0X40, 0X80
};

static unsigned char
mapDots (unsigned char input, const DotsTable from, const DotsTable to) {
  unsigned char output = 0;
  {
    int dot;
    for (dot=0; dot<DOTS_TABLE_SIZE; ++dot) {
      if (input & from[dot]) output |= to[dot];
    }
  }
  return output;
}

static int
readTable_tbl (const char *path, FILE *file, TranslationTable table, void *data) {
  return loadTranslationTable(path, file, table,
                              TBL_UNDEFINED | TBL_DUPLICATE | TBL_UNUSED);
}

static int
writeTable_tbl (const char *path, FILE *file, const TranslationTable table, void *data) {
  int index;
  for (index=0; index<TRANSLATION_TABLE_SIZE; ++index) {
    unsigned char cell = table[index];
    if (fprintf(file, "\\X%02X (", index) == EOF) goto error;

#define DOT(dot) if (fputs(((cell & BRL_DOT##dot)? #dot: " "), file) == EOF) goto error
    DOT(1);
    DOT(2);
    DOT(3);
    DOT(4);
    DOT(5);
    DOT(6);
    DOT(7);
    DOT(8);
#undef DOT

    if (fprintf(file, ")\n") == EOF) goto error;
  }
  return 1;

error:
  return 0;
}

static int
readTable_bin (const char *path, FILE *file, TranslationTable table, void *data) {
  {
    int character;
    for (character=0; character<TRANSLATION_TABLE_SIZE; ++character) {
      int cell = fgetc(file);

      if (cell == EOF) {
        if (ferror(file)) {
          LogPrint(LOG_ERR, "input error: %s: %s", path, strerror(errno));
        } else {
          LogPrint(LOG_ERR, "table too short: %s", path);
        }
        return 0;
      }

      if (data) cell = mapDots(cell, data, dotsInternal);
      table[character] = cell;
    }
  }

  return 1;
}

static int
writeTable_bin (const char *path, FILE *file, const TranslationTable table, void *data) {
  {
    int character;
    for (character=0; character<TRANSLATION_TABLE_SIZE; ++character) {
      unsigned char cell = table[character];
      if (data) cell = mapDots(cell, dotsInternal, data);
      if (fputc(cell, file) == EOF) {
        LogPrint(LOG_ERR, "output error: %s: %s", path, strerror(errno));
        return 0;
      }
    }
  }

  return 1;
}

typedef int TableReader (const char *path, FILE *file, TranslationTable table, void *data);
typedef int TableWriter (const char *path, FILE *file, const TranslationTable table, void *data);
typedef struct {
  const char *name;
  TableReader *read;
  TableWriter *write;
  void *data;
} FormatEntry;
static const FormatEntry formatEntries[] = {
  {"tbl", readTable_tbl, writeTable_tbl, NULL},
  {"a2b", readTable_bin, writeTable_bin, &dots12345678},
  {NULL}
};

static const FormatEntry *
findFormatEntry (const char *name) {
  const FormatEntry *format = formatEntries;
  while (format->name) {
    if (strcmp(name, format->name) == 0) return format;
    ++format;
  }
  return NULL;
}

static const char *
findFileExtension (const char *path) {
  const char *name = strrchr(path, '/');
  return strrchr((name? name: path), '.');
}

static const FormatEntry *
getFormatEntry (const char *name, const char *path, const char *description) {
  if (!(name && *name)) {
    name = findFileExtension(path);
    if (!(name && *++name)) {
      LogPrint(LOG_ERR, "unspecified %s format.", description);
      exit(2);
    }
  }

  {
    const FormatEntry *format = findFormatEntry(name);
    if (format) return format;
  }

  LogPrint(LOG_ERR, "unknown %s format: %s", description, name);
  exit(2);
}

static FILE *
openTable (const char **file, const char *mode, const char *directory, FILE *stdStream, const char *stdName) {
  if (stdStream) {
    if (strcmp(*file, "-") == 0) {
      *file = stdName;
      return stdStream;
    }
  }

  if (directory) {
    const char *path = makePath(directory, *file);
    if (!path) return NULL;
    *file = path;
  }

  {
    FILE *stream = fopen(*file, mode);
    if (!stream) LogPrint(LOG_ERR, "table open error: %s: %s", *file, strerror(errno));
    return stream;
  }
}

int
main (int argc, char *argv[]) {
  int status;
  const char *inputPath;
  const char *outputPath;
  const FormatEntry *inputFormat;
  const FormatEntry *outputFormat;

  processOptions(optionTable, optionCount,
                 "tbltest", &argc, &argv,
                 NULL, NULL, NULL,
                 "input-table [output-table]");

  if (argc == 0) {
    LogPrint(LOG_ERR, "missing input table.");
    exit(2);
  }
  inputPath = *argv++, argc--;

  if (argc > 0) {
    outputPath = *argv++, argc--;
  } else if (opt_outputFormat && *opt_outputFormat) {
    const char *extension = findFileExtension(inputPath);
    int prefix = extension? (extension - inputPath): strlen(inputPath);
    char buffer[prefix + 1 + strlen(opt_outputFormat) + 1];
    snprintf(buffer, sizeof(buffer), "%.*s.%s", prefix, inputPath, opt_outputFormat);
    outputPath = strdupWrapper(buffer);
  } else {
    outputPath = NULL;
  }

  if (argc > 0) {
    LogPrint(LOG_ERR, "too many parameters.");
    exit(2);
  }

  inputFormat = getFormatEntry(opt_inputFormat, inputPath, "input");
  if (outputPath) {
    outputFormat = getFormatEntry(opt_outputFormat, outputPath, "output");
  } else {
    outputFormat = NULL;
  }

  if (opt_translate) {
    FILE *inputFile = openTable(&inputPath, "r", opt_dataDirectory, NULL, NULL);

    if (inputFile) {
      TranslationTable inputTable;

      if (inputFormat->read(inputPath, inputFile, inputTable, inputFormat->data)) {
        if (outputPath) {
          FILE *outputFile = openTable(&outputPath, "r", opt_dataDirectory, NULL, NULL);

          if (outputFile) {
            TranslationTable outputTable;

            if (outputFormat->read(outputPath, outputFile, outputTable, outputFormat->data)) {
              TranslationTable table;

              {
                TranslationTable unoutputTable;
                int byte;
                reverseTranslationTable(outputTable, unoutputTable);
                memset(&table, 0, sizeof(table));
                for (byte=TRANSLATION_TABLE_SIZE-1; byte>=0; byte--)
                  table[byte] = unoutputTable[inputTable[byte]];
              }

              status = 0;
              while (1) {
                int character;

                if ((character = fgetc(stdin)) == EOF) {
                  if (ferror(stdin)) {
                    LogPrint(LOG_ERR, "input error: %s", strerror(errno));
                    status = 6;
                  }
                  break;
                }

                if (fputc(table[character], stdout) == EOF) {
                  LogPrint(LOG_ERR, "output error: %s", strerror(errno));
                  status = 7;
                  break;
                }
              }
            } else {
              status = 4;
            }

            if (fclose(outputFile) == EOF) {
              if (!status) {
                LogPrint(LOG_ERR, "output error: %s", strerror(errno));
                status = 7;
              }
            }
          } else {
            status = 3;
          }
        } else {
          LogPrint(LOG_ERR, "output table not specified.");
          status = 2;
        }
      } else {
        status = 4;
      }

      fclose(inputFile);
    } else {
      status = 3;
    }
  } else {
    if (outputFormat != inputFormat) {
      FILE *inputFile = openTable(&inputPath, "r", opt_dataDirectory, stdin, "<standard-input>");

      if (inputFile) {
        TranslationTable table;

        if (inputFormat->read(inputPath, inputFile, table, inputFormat->data)) {
          if (outputPath) {
            FILE *outputFile = openTable(&outputPath, "w", NULL, stdout, "<standard-output>");

            if (outputFile) {
              if (outputFormat->write(outputPath, outputFile, table, outputFormat->data)) {
                status = 0;
              } else {
                status = 6;
              }

              fclose(outputFile);
            } else {
              status = 5;
            }
          } else {
            status = 0;
          }
        } else {
          status = 4;
        }

        fclose(inputFile);
      } else {
        status = 3;
      }
    } else {
      LogPrint(LOG_ERR, "same input and output formats: %s", outputFormat->name);
      status = 2;
    }
  }

  return status;
}
