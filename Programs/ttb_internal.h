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
 * Foundation.  Please see the file COPYING for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#ifndef BRLTTY_INCLUDED_TTB_INTERNAL
#define BRLTTY_INCLUDED_TTB_INTERNAL

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef uint32_t TextTableOffset;

#define BYTES_PER_CHARSET 0X100

#define UNICODE_CELL_BITS 8
#define UNICODE_ROW_BITS 8
#define UNICODE_PLAIN_BITS 8
#define UNICODE_GROUP_BITS 7

#define UNICODE_CELLS_PER_ROW (1 << UNICODE_CELL_BITS)
#define UNICODE_ROWS_PER_PLAIN (1 << UNICODE_ROW_BITS)
#define UNICODE_PLAINS_PER_GROUP (1 << UNICODE_PLAIN_BITS)
#define UNICODE_GROUP_COUNT (1 << UNICODE_GROUP_BITS)

#define UNICODE_CELL_MASK (UNICODE_CELLS_PER_ROW - 1)
//#define UNICODE_CHARACTER_MASK ((1 << (UNICODE_CELL_BITS + UNICODE_ROW_BITS + UNICODE_PLAIN_BITS + UNICODE_GROUP_BITS)) - 1)
#define UNICODE_CHARACTER_MASK 0X7FFFFFFF

typedef struct {
  unsigned char dots;
} UnicodeCellEntry;

typedef struct {
  UnicodeCellEntry cells[UNICODE_CELLS_PER_ROW];
} UnicodeRowEntry;

typedef struct {
  TextTableOffset rows[UNICODE_ROWS_PER_PLAIN];
} UnicodePlainEntry;

typedef struct {
  TextTableOffset plains[UNICODE_PLAINS_PER_GROUP];
} UnicodeGroupEntry;

typedef struct {
  unsigned char byteToDots[BYTES_PER_CHARSET];
  wchar_t dotsToCharacter[0X100];
  TextTableOffset unicodeGroups[UNICODE_GROUP_COUNT];
} TextTableHeader;

struct TextTableStruct {
  union {
    TextTableHeader *fields;
    const unsigned char *bytes;
  } header;

  size_t size;
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BRLTTY_INCLUDED_TTB_INTERNAL */
