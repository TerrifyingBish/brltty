/*
 * BRLTTY - A background process providing access to the Linux console (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2004 by The BRLTTY Team. All rights reserved.
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

/*
#    Name:     cp864_DOSArabic to Unicode table
#    Unicode version: 2.0
#    Table version: 2.00
#    Table format:  Format A
#    Date:          04/24/96
#    Authors:       Lori Brownell <loribr@microsoft.com>
#                   K.D. Chang    <a-kchang@microsoft.com>
#    General notes: none
#
#    Format: Three tab-separated columns
#        Column #1 is the cp864_DOSArabic code (in hex)
#        Column #2 is the Unicode (in hex as 0xXXXX)
#        Column #3 is the Unicode name (follows a comment sign, '#')
#
#    The entries are in cp864_DOSArabic order
*/
[0x00]=0x0000, //	NULL
[0x01]=0x0001, //	START OF HEADING
[0x02]=0x0002, //	START OF TEXT
[0x03]=0x0003, //	END OF TEXT
[0x04]=0x0004, //	END OF TRANSMISSION
[0x05]=0x0005, //	ENQUIRY
[0x06]=0x0006, //	ACKNOWLEDGE
[0x07]=0x0007, //	BELL
[0x08]=0x0008, //	BACKSPACE
[0x09]=0x0009, //	HORIZONTAL TABULATION
[0x0a]=0x000a, //	LINE FEED
[0x0b]=0x000b, //	VERTICAL TABULATION
[0x0c]=0x000c, //	FORM FEED
[0x0d]=0x000d, //	CARRIAGE RETURN
[0x0e]=0x000e, //	SHIFT OUT
[0x0f]=0x000f, //	SHIFT IN
[0x10]=0x0010, //	DATA LINK ESCAPE
[0x11]=0x0011, //	DEVICE CONTROL ONE
[0x12]=0x0012, //	DEVICE CONTROL TWO
[0x13]=0x0013, //	DEVICE CONTROL THREE
[0x14]=0x0014, //	DEVICE CONTROL FOUR
[0x15]=0x0015, //	NEGATIVE ACKNOWLEDGE
[0x16]=0x0016, //	SYNCHRONOUS IDLE
[0x17]=0x0017, //	END OF TRANSMISSION BLOCK
[0x18]=0x0018, //	CANCEL
[0x19]=0x0019, //	END OF MEDIUM
[0x1a]=0x001a, //	SUBSTITUTE
[0x1b]=0x001b, //	ESCAPE
[0x1c]=0x001c, //	FILE SEPARATOR
[0x1d]=0x001d, //	GROUP SEPARATOR
[0x1e]=0x001e, //	RECORD SEPARATOR
[0x1f]=0x001f, //	UNIT SEPARATOR
[0x20]=0x0020, //	SPACE
[0x21]=0x0021, //	EXCLAMATION MARK
[0x22]=0x0022, //	QUOTATION MARK
[0x23]=0x0023, //	NUMBER SIGN
[0x24]=0x0024, //	DOLLAR SIGN
[0x25]=0x066a, //	ARABIC PERCENT SIGN
[0x26]=0x0026, //	AMPERSAND
[0x27]=0x0027, //	APOSTROPHE
[0x28]=0x0028, //	LEFT PARENTHESIS
[0x29]=0x0029, //	RIGHT PARENTHESIS
[0x2a]=0x002a, //	ASTERISK
[0x2b]=0x002b, //	PLUS SIGN
[0x2c]=0x002c, //	COMMA
[0x2d]=0x002d, //	HYPHEN-MINUS
[0x2e]=0x002e, //	FULL STOP
[0x2f]=0x002f, //	SOLIDUS
[0x30]=0x0030, //	DIGIT ZERO
[0x31]=0x0031, //	DIGIT ONE
[0x32]=0x0032, //	DIGIT TWO
[0x33]=0x0033, //	DIGIT THREE
[0x34]=0x0034, //	DIGIT FOUR
[0x35]=0x0035, //	DIGIT FIVE
[0x36]=0x0036, //	DIGIT SIX
[0x37]=0x0037, //	DIGIT SEVEN
[0x38]=0x0038, //	DIGIT EIGHT
[0x39]=0x0039, //	DIGIT NINE
[0x3a]=0x003a, //	COLON
[0x3b]=0x003b, //	SEMICOLON
[0x3c]=0x003c, //	LESS-THAN SIGN
[0x3d]=0x003d, //	EQUALS SIGN
[0x3e]=0x003e, //	GREATER-THAN SIGN
[0x3f]=0x003f, //	QUESTION MARK
[0x40]=0x0040, //	COMMERCIAL AT
[0x41]=0x0041, //	LATIN CAPITAL LETTER A
[0x42]=0x0042, //	LATIN CAPITAL LETTER B
[0x43]=0x0043, //	LATIN CAPITAL LETTER C
[0x44]=0x0044, //	LATIN CAPITAL LETTER D
[0x45]=0x0045, //	LATIN CAPITAL LETTER E
[0x46]=0x0046, //	LATIN CAPITAL LETTER F
[0x47]=0x0047, //	LATIN CAPITAL LETTER G
[0x48]=0x0048, //	LATIN CAPITAL LETTER H
[0x49]=0x0049, //	LATIN CAPITAL LETTER I
[0x4a]=0x004a, //	LATIN CAPITAL LETTER J
[0x4b]=0x004b, //	LATIN CAPITAL LETTER K
[0x4c]=0x004c, //	LATIN CAPITAL LETTER L
[0x4d]=0x004d, //	LATIN CAPITAL LETTER M
[0x4e]=0x004e, //	LATIN CAPITAL LETTER N
[0x4f]=0x004f, //	LATIN CAPITAL LETTER O
[0x50]=0x0050, //	LATIN CAPITAL LETTER P
[0x51]=0x0051, //	LATIN CAPITAL LETTER Q
[0x52]=0x0052, //	LATIN CAPITAL LETTER R
[0x53]=0x0053, //	LATIN CAPITAL LETTER S
[0x54]=0x0054, //	LATIN CAPITAL LETTER T
[0x55]=0x0055, //	LATIN CAPITAL LETTER U
[0x56]=0x0056, //	LATIN CAPITAL LETTER V
[0x57]=0x0057, //	LATIN CAPITAL LETTER W
[0x58]=0x0058, //	LATIN CAPITAL LETTER X
[0x59]=0x0059, //	LATIN CAPITAL LETTER Y
[0x5a]=0x005a, //	LATIN CAPITAL LETTER Z
[0x5b]=0x005b, //	LEFT SQUARE BRACKET
[0x5c]=0x005c, //	REVERSE SOLIDUS
[0x5d]=0x005d, //	RIGHT SQUARE BRACKET
[0x5e]=0x005e, //	CIRCUMFLEX ACCENT
[0x5f]=0x005f, //	LOW LINE
[0x60]=0x0060, //	GRAVE ACCENT
[0x61]=0x0061, //	LATIN SMALL LETTER A
[0x62]=0x0062, //	LATIN SMALL LETTER B
[0x63]=0x0063, //	LATIN SMALL LETTER C
[0x64]=0x0064, //	LATIN SMALL LETTER D
[0x65]=0x0065, //	LATIN SMALL LETTER E
[0x66]=0x0066, //	LATIN SMALL LETTER F
[0x67]=0x0067, //	LATIN SMALL LETTER G
[0x68]=0x0068, //	LATIN SMALL LETTER H
[0x69]=0x0069, //	LATIN SMALL LETTER I
[0x6a]=0x006a, //	LATIN SMALL LETTER J
[0x6b]=0x006b, //	LATIN SMALL LETTER K
[0x6c]=0x006c, //	LATIN SMALL LETTER L
[0x6d]=0x006d, //	LATIN SMALL LETTER M
[0x6e]=0x006e, //	LATIN SMALL LETTER N
[0x6f]=0x006f, //	LATIN SMALL LETTER O
[0x70]=0x0070, //	LATIN SMALL LETTER P
[0x71]=0x0071, //	LATIN SMALL LETTER Q
[0x72]=0x0072, //	LATIN SMALL LETTER R
[0x73]=0x0073, //	LATIN SMALL LETTER S
[0x74]=0x0074, //	LATIN SMALL LETTER T
[0x75]=0x0075, //	LATIN SMALL LETTER U
[0x76]=0x0076, //	LATIN SMALL LETTER V
[0x77]=0x0077, //	LATIN SMALL LETTER W
[0x78]=0x0078, //	LATIN SMALL LETTER X
[0x79]=0x0079, //	LATIN SMALL LETTER Y
[0x7a]=0x007a, //	LATIN SMALL LETTER Z
[0x7b]=0x007b, //	LEFT CURLY BRACKET
[0x7c]=0x007c, //	VERTICAL LINE
[0x7d]=0x007d, //	RIGHT CURLY BRACKET
[0x7e]=0x007e, //	TILDE
[0x7f]=0x007f, //	DELETE
[0x80]=0x00b0, //	DEGREE SIGN
[0x81]=0x00b7, //	MIDDLE DOT
[0x82]=0x2219, //	BULLET OPERATOR
[0x83]=0x221a, //	SQUARE ROOT
[0x84]=0x2592, //	MEDIUM SHADE
[0x85]=0x2500, //	FORMS LIGHT HORIZONTAL
[0x86]=0x2502, //	FORMS LIGHT VERTICAL
[0x87]=0x253c, //	FORMS LIGHT VERTICAL AND HORIZONTAL
[0x88]=0x2524, //	FORMS LIGHT VERTICAL AND LEFT
[0x89]=0x252c, //	FORMS LIGHT DOWN AND HORIZONTAL
[0x8a]=0x251c, //	FORMS LIGHT VERTICAL AND RIGHT
[0x8b]=0x2534, //	FORMS LIGHT UP AND HORIZONTAL
[0x8c]=0x2510, //	FORMS LIGHT DOWN AND LEFT
[0x8d]=0x250c, //	FORMS LIGHT DOWN AND RIGHT
[0x8e]=0x2514, //	FORMS LIGHT UP AND RIGHT
[0x8f]=0x2518, //	FORMS LIGHT UP AND LEFT
[0x90]=0x03b2, //	GREEK SMALL BETA
[0x91]=0x221e, //	INFINITY
[0x92]=0x03c6, //	GREEK SMALL PHI
[0x93]=0x00b1, //	PLUS-OR-MINUS SIGN
[0x94]=0x00bd, //	FRACTION 1/2
[0x95]=0x00bc, //	FRACTION 1/4
[0x96]=0x2248, //	ALMOST EQUAL TO
[0x97]=0x00ab, //	LEFT POINTING GUILLEMET
[0x98]=0x00bb, //	RIGHT POINTING GUILLEMET
[0x99]=0xfef7, //	ARABIC LIGATURE LAM WITH ALEF WITH HAMZA ABOVE ISOLATED FORM
[0x9a]=0xfef8, //	ARABIC LIGATURE LAM WITH ALEF WITH HAMZA ABOVE FINAL FORM
[0x9d]=0xfefb, //	ARABIC LIGATURE LAM WITH ALEF ISOLATED FORM
[0x9e]=0xfefc, //	ARABIC LIGATURE LAM WITH ALEF FINAL FORM
[0xa0]=0x00a0, //	NON-BREAKING SPACE
[0xa1]=0x00ad, //	SOFT HYPHEN
[0xa2]=0xfe82, //	ARABIC LETTER ALEF WITH MADDA ABOVE FINAL FORM
[0xa3]=0x00a3, //	POUND SIGN
[0xa4]=0x00a4, //	CURRENCY SIGN
[0xa5]=0xfe84, //	ARABIC LETTER ALEF WITH HAMZA ABOVE FINAL FORM
[0xa8]=0xfe8e, //	ARABIC LETTER ALEF FINAL FORM
[0xa9]=0xfe8f, //	ARABIC LETTER BEH ISOLATED FORM
[0xaa]=0xfe95, //	ARABIC LETTER TEH ISOLATED FORM
[0xab]=0xfe99, //	ARABIC LETTER THEH ISOLATED FORM
[0xac]=0x060c, //	ARABIC COMMA
[0xad]=0xfe9d, //	ARABIC LETTER JEEM ISOLATED FORM
[0xae]=0xfea1, //	ARABIC LETTER HAH ISOLATED FORM
[0xaf]=0xfea5, //	ARABIC LETTER KHAH ISOLATED FORM
[0xb0]=0x0660, //	ARABIC-INDIC DIGIT ZERO
[0xb1]=0x0661, //	ARABIC-INDIC DIGIT ONE
[0xb2]=0x0662, //	ARABIC-INDIC DIGIT TWO
[0xb3]=0x0663, //	ARABIC-INDIC DIGIT THREE
[0xb4]=0x0664, //	ARABIC-INDIC DIGIT FOUR
[0xb5]=0x0665, //	ARABIC-INDIC DIGIT FIVE
[0xb6]=0x0666, //	ARABIC-INDIC DIGIT SIX
[0xb7]=0x0667, //	ARABIC-INDIC DIGIT SEVEN
[0xb8]=0x0668, //	ARABIC-INDIC DIGIT EIGHT
[0xb9]=0x0669, //	ARABIC-INDIC DIGIT NINE
[0xba]=0xfed1, //	ARABIC LETTER FEH ISOLATED FORM
[0xbb]=0x061b, //	ARABIC SEMICOLON
[0xbc]=0xfeb1, //	ARABIC LETTER SEEN ISOLATED FORM
[0xbd]=0xfeb5, //	ARABIC LETTER SHEEN ISOLATED FORM
[0xbe]=0xfeb9, //	ARABIC LETTER SAD ISOLATED FORM
[0xbf]=0x061f, //	ARABIC QUESTION MARK
[0xc0]=0x00a2, //	CENT SIGN
[0xc1]=0xfe80, //	ARABIC LETTER HAMZA ISOLATED FORM
[0xc2]=0xfe81, //	ARABIC LETTER ALEF WITH MADDA ABOVE ISOLATED FORM
[0xc3]=0xfe83, //	ARABIC LETTER ALEF WITH HAMZA ABOVE ISOLATED FORM
[0xc4]=0xfe85, //	ARABIC LETTER WAW WITH HAMZA ABOVE ISOLATED FORM
[0xc5]=0xfeca, //	ARABIC LETTER AIN FINAL FORM
[0xc6]=0xfe8b, //	ARABIC LETTER YEH WITH HAMZA ABOVE INITIAL FORM
[0xc7]=0xfe8d, //	ARABIC LETTER ALEF ISOLATED FORM
[0xc8]=0xfe91, //	ARABIC LETTER BEH INITIAL FORM
[0xc9]=0xfe93, //	ARABIC LETTER TEH MARBUTA ISOLATED FORM
[0xca]=0xfe97, //	ARABIC LETTER TEH INITIAL FORM
[0xcb]=0xfe9b, //	ARABIC LETTER THEH INITIAL FORM
[0xcc]=0xfe9f, //	ARABIC LETTER JEEM INITIAL FORM
[0xcd]=0xfea3, //	ARABIC LETTER HAH INITIAL FORM
[0xce]=0xfea7, //	ARABIC LETTER KHAH INITIAL FORM
[0xcf]=0xfea9, //	ARABIC LETTER DAL ISOLATED FORM
[0xd0]=0xfeab, //	ARABIC LETTER THAL ISOLATED FORM
[0xd1]=0xfead, //	ARABIC LETTER REH ISOLATED FORM
[0xd2]=0xfeaf, //	ARABIC LETTER ZAIN ISOLATED FORM
[0xd3]=0xfeb3, //	ARABIC LETTER SEEN INITIAL FORM
[0xd4]=0xfeb7, //	ARABIC LETTER SHEEN INITIAL FORM
[0xd5]=0xfebb, //	ARABIC LETTER SAD INITIAL FORM
[0xd6]=0xfebf, //	ARABIC LETTER DAD INITIAL FORM
[0xd7]=0xfec1, //	ARABIC LETTER TAH ISOLATED FORM
[0xd8]=0xfec5, //	ARABIC LETTER ZAH ISOLATED FORM
[0xd9]=0xfecb, //	ARABIC LETTER AIN INITIAL FORM
[0xda]=0xfecf, //	ARABIC LETTER GHAIN INITIAL FORM
[0xdb]=0x00a6, //	BROKEN VERTICAL BAR
[0xdc]=0x00ac, //	NOT SIGN
[0xdd]=0x00f7, //	DIVISION SIGN
[0xde]=0x00d7, //	MULTIPLICATION SIGN
[0xdf]=0xfec9, //	ARABIC LETTER AIN ISOLATED FORM
[0xe0]=0x0640, //	ARABIC TATWEEL
[0xe1]=0xfed3, //	ARABIC LETTER FEH INITIAL FORM
[0xe2]=0xfed7, //	ARABIC LETTER QAF INITIAL FORM
[0xe3]=0xfedb, //	ARABIC LETTER KAF INITIAL FORM
[0xe4]=0xfedf, //	ARABIC LETTER LAM INITIAL FORM
[0xe5]=0xfee3, //	ARABIC LETTER MEEM INITIAL FORM
[0xe6]=0xfee7, //	ARABIC LETTER NOON INITIAL FORM
[0xe7]=0xfeeb, //	ARABIC LETTER HEH INITIAL FORM
[0xe8]=0xfeed, //	ARABIC LETTER WAW ISOLATED FORM
[0xe9]=0xfeef, //	ARABIC LETTER ALEF MAKSURA ISOLATED FORM
[0xea]=0xfef3, //	ARABIC LETTER YEH INITIAL FORM
[0xeb]=0xfebd, //	ARABIC LETTER DAD ISOLATED FORM
[0xec]=0xfecc, //	ARABIC LETTER AIN MEDIAL FORM
[0xed]=0xfece, //	ARABIC LETTER GHAIN FINAL FORM
[0xee]=0xfecd, //	ARABIC LETTER GHAIN ISOLATED FORM
[0xef]=0xfee1, //	ARABIC LETTER MEEM ISOLATED FORM
[0xf0]=0xfe7d, //	ARABIC SHADDA MEDIAL FORM
[0xf1]=0x0651, //	ARABIC SHADDAH
[0xf2]=0xfee5, //	ARABIC LETTER NOON ISOLATED FORM
[0xf3]=0xfee9, //	ARABIC LETTER HEH ISOLATED FORM
[0xf4]=0xfeec, //	ARABIC LETTER HEH MEDIAL FORM
[0xf5]=0xfef0, //	ARABIC LETTER ALEF MAKSURA FINAL FORM
[0xf6]=0xfef2, //	ARABIC LETTER YEH FINAL FORM
[0xf7]=0xfed0, //	ARABIC LETTER GHAIN MEDIAL FORM
[0xf8]=0xfed5, //	ARABIC LETTER QAF ISOLATED FORM
[0xf9]=0xfef5, //	ARABIC LIGATURE LAM WITH ALEF WITH MADDA ABOVE ISOLATED FORM
[0xfa]=0xfef6, //	ARABIC LIGATURE LAM WITH ALEF WITH MADDA ABOVE FINAL FORM
[0xfb]=0xfedd, //	ARABIC LETTER LAM ISOLATED FORM
[0xfc]=0xfed9, //	ARABIC LETTER KAF ISOLATED FORM
[0xfd]=0xfef1, //	ARABIC LETTER YEH ISOLATED FORM
[0xfe]=0x25a0  //	BLACK SQUARE
