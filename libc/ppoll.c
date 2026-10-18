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
 * ppoll() for the PS4. FreeBSD's ppoll system call (545) came with
 * FreeBSD 11; on the PS4's FreeBSD 9 kernel that number is one of Sony's.
 * The signal mask is set around poll() instead, which, unlike the system
 * call, does not set it and wait atomically: a signal that arrives between
 * sigprocmask() and poll() is handled before poll() starts waiting.
 **/

#include <errno.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <time.h>


int
ppoll(struct pollfd fds[], nfds_t nfds,
      const struct timespec *restrict timeout,
      const sigset_t *restrict newsigmask) {
  sigset_t oldsigmask;
  int timeout_ms = -1;
  int saved_errno;
  int ret;

  if(timeout) {
    if(timeout->tv_sec < 0 || timeout->tv_nsec < 0 ||
       timeout->tv_nsec >= 1000000000L) {
      errno = EINVAL;
      return -1;
    }
    if(timeout->tv_sec >= INT_MAX / 1000) {
      timeout_ms = INT_MAX;
    } else {
      // round up, so that poll() waits at least as long as asked
      timeout_ms = timeout->tv_sec * 1000 +
	(timeout->tv_nsec + 999999) / 1000000;
    }
  }

  if(newsigmask && sigprocmask(SIG_SETMASK, newsigmask, &oldsigmask)) {
    return -1;
  }

  ret = poll(fds, nfds, timeout_ms);

  if(newsigmask) {
    saved_errno = errno;
    sigprocmask(SIG_SETMASK, &oldsigmask, 0);
    errno = saved_errno;
  }

  return ret;
}
