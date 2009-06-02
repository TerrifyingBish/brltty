/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2009 by The BRLTTY Developers.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any
 * later version. Please see the file LICENSE-GPL for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#include "prologue.h"

#include <string.h>

#include "misc.h"
#include "ktb.h"
#include "ktb_internal.h"
#include "cmd.h"
#include "brldefs.h"

static inline const void *
getKeyTableItem (KeyTable *table, KeyTableOffset offset) {
  return &table->header.bytes[offset];
}

int
getKeyCommand (KeyTable *table, const KeyCodeSet *modifiers, KeyCode code) {
  const KeyTableHeader *header = table->header.fields;
  const KeyBinding *binding = getKeyTableItem(table, header->bindingsTable);
  unsigned int count = header->bindingsCount;

  while (count) {
    if ((code == binding->key.code) &&
        sameKeyCodeMasks(modifiers->mask, binding->key.modifiers))
      return binding->command;
    binding += 1, count -= 1;
  }

  return EOF;
}

int
isKeyModifiers (KeyTable *table, const KeyCodeSet *modifiers) {
  const KeyTableHeader *header = table->header.fields;
  const KeyBinding *binding = getKeyTableItem(table, header->bindingsTable);
  unsigned int count = header->bindingsCount;

  while (count) {
    if (isKeySubset(binding->key.modifiers, modifiers->mask)) return 1;
    binding += 1, count -= 1;
  }

  return 0;
}

void
resetKeyTable (KeyTable *table) {
  removeAllKeyCodes(&table->keyCodeSet);
  table->lastCommand = EOF;
}

KeyTableState
processKeyEvent (KeyTable *keyTable, KeyCode code, int press) {
  removeKeyCode(&keyTable->keyCodeSet, code);

  {
    int command = getKeyCommand(keyTable, &keyTable->keyCodeSet, code);
    int bound = command != EOF;
    if (press) addKeyCode(&keyTable->keyCodeSet, code);

    if (!press || !bound) {
      if (keyTable->lastCommand != EOF) {
        keyTable->lastCommand = EOF;
        enqueueCommand(BRL_CMD_NOOP);
      }
    } else if (command != keyTable->lastCommand) {
      keyTable->lastCommand = command;
      enqueueCommand(command | BRL_FLG_REPEAT_INITIAL | BRL_FLG_REPEAT_DELAY);
    }

    if (bound) return KTS_YES;
  }

  return isKeyModifiers(keyTable, &keyTable->keyCodeSet)? KTS_MAYBE: KTS_NO;
}
