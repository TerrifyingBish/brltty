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

#ifndef _ROUTE_H
#define _ROUTE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern volatile int csr_active;
extern volatile pid_t csr_pid;

int csrjmp (int x, int y, int scrno);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ROUTE_H */
