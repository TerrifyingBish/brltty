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

#ifndef _SPK_H
#define _SPK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* spk.h - Header file for the speech library
 */

typedef struct {
  const char *name;
  const char *identifier;
  const char *const *parameters;
  void (*identify) (void);		/* print start-up messages */
  int (*open) (char **parameters);		/* initialize speech device */
  void (*say) (const unsigned char *buffer, int len);	/* speak text */
  void (*mute) (void);		/* mute speech */
  void (*close) (void);		/* close speech device */

  /* These require SPK_HAVE_EXPRESS. */
  void (*express) (const unsigned char *buffer, int len);	/* speak text */

  /* These require SPK_HAVE_TRACK. */
  void (*doTrack) (void);		/* get current speaking position */
  int (*getTrack) (void);		/* get current speaking position */
  int (*isSpeaking) (void);		/* get current speaking position */
} SpeechDriver;

extern const SpeechDriver *loadSpeechDriver (const char *driver, const char *driverDirectory);
extern int listSpeechDrivers (const char *directory);
extern const SpeechDriver *speech;
extern const SpeechDriver noSpeech;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPK_H */
