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

#include <stdarg.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/syscall.h>


/**
 * The kernel entry point (__semctl) takes a pointer to union semun,
 * while the userspace API passes the union by value as a variadic argument.
 **/
int
semctl(int semid, int semnum, int cmd, ...) {
  union semun *semun_ptr = 0;
  union semun semun;
  va_list ap;

  va_start(ap, cmd);
  if(cmd == IPC_SET || cmd == IPC_STAT || cmd == GETALL ||
     cmd == SETVAL || cmd == SETALL) {
    semun = va_arg(ap, union semun);
    semun_ptr = &semun;
  }
  va_end(ap);

  return syscall(SYS___semctl, semid, semnum, cmd, semun_ptr);
}
