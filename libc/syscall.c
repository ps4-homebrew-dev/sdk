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


#include <errno.h>
#include <unistd.h>


/**
 * Make system call number n with up to six arguments, as FreeBSD's
 * __syscall() does: with the syscall instruction, which the PS4 lets a
 * process run from its own code. On failure the kernel sets the carry
 * flag and returns the error number; we set errno and return -1.
 **/
long
__syscall(long n, ...) {
  long a1, a2, a3, a4, a5, a6;
  __builtin_va_list ap;
  long ret;
  char err;

  __builtin_va_start(ap, n);
  a1 = __builtin_va_arg(ap, long);
  a2 = __builtin_va_arg(ap, long);
  a3 = __builtin_va_arg(ap, long);
  a4 = __builtin_va_arg(ap, long);
  a5 = __builtin_va_arg(ap, long);
  a6 = __builtin_va_arg(ap, long);
  __builtin_va_end(ap);

  register long r10 asm("r10") = a4;
  register long r8  asm("r8")  = a5;
  register long r9  asm("r9")  = a6;

  asm volatile("syscall"
	       : "=a"(ret), "=@ccc"(err), "+r"(r10), "+r"(r8), "+r"(r9)
	       : "a"(n), "D"(a1), "S"(a2), "d"(a3)
	       : "rcx", "r11", "memory");

  if(err) {
    errno = (int)ret;
    return -1;
  }

  return ret;
}


int
syscall(int n, ...) {
  long a1, a2, a3, a4, a5, a6;
  __builtin_va_list ap;

  __builtin_va_start(ap, n);
  a1 = __builtin_va_arg(ap, long);
  a2 = __builtin_va_arg(ap, long);
  a3 = __builtin_va_arg(ap, long);
  a4 = __builtin_va_arg(ap, long);
  a5 = __builtin_va_arg(ap, long);
  a6 = __builtin_va_arg(ap, long);
  __builtin_va_end(ap);

  return (int)__syscall(n, a1, a2, a3, a4, a5, a6);
}
