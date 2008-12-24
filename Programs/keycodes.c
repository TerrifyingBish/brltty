/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2008 by The BRLTTY Developers.
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

#include "keycodes.h"

void
copyKeyCodeMask (KeyCodeMask to, const KeyCodeMask from) {
  memcpy(to, from, KEY_CODE_MASK_SIZE);
}

int
sameKeyCodeMasks (const KeyCodeMask mask1, const KeyCodeMask mask2) {
  return memcmp(mask1, mask2, KEY_CODE_MASK_SIZE) == 0;
}

int
isKeySubset (const KeyCodeMask set, const KeyCodeMask subset) {
  unsigned int count = KEY_CODE_MASK_ELEMENT_COUNT;

  while (count) {
    if (~*set & *subset) return 0;
    set += 1, subset += 1, count -= 1;
  }

  return 1;
}
