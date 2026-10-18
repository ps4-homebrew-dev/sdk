/* Copyright (C) 2026 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */

/**
 * The C11 thread types, for libc's call_once.c, cnd.c, mtx.c, thrd.c and
 * tss.c. The SDK's headers are FreeBSD 9's, which has no <threads.h>; the
 * types are the pthread types the functions wrap, as on FreeBSD, and the
 * constants FreeBSD's.
 **/

#ifndef _LIBC_THREADS_H_
#define _LIBC_THREADS_H_

#include <sys/_pthreadtypes.h>
#include <time.h>

typedef pthread_cond_t  cnd_t;
typedef pthread_mutex_t mtx_t;
typedef pthread_t       thrd_t;
typedef pthread_key_t   tss_t;
typedef pthread_once_t  once_flag;

typedef void (*tss_dtor_t)(void *);
typedef int (*thrd_start_t)(void *);

enum {
  mtx_plain = 0x1,
  mtx_recursive = 0x2,
  mtx_timed = 0x4
};

enum {
  thrd_busy = 1,
  thrd_error = 2,
  thrd_nomem = 3,
  thrd_success = 4,
  thrd_timedout = 5
};

#define TSS_DTOR_ITERATIONS 4

#endif /* _LIBC_THREADS_H_ */
