/* Copyright (C) 2026 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */

#include "rtld.h"


/**
 * Dependencies provided by the ELF linker.
 **/
extern void (*__preinit_array_start[])(void) __attribute__((weak, visibility("hidden")));
extern void (*__preinit_array_end[])(void) __attribute__((weak, visibility("hidden")));

extern void (*__init_array_start[])(void) __attribute__((weak, visibility("hidden")));
extern void (*__init_array_end[])(void) __attribute__((weak, visibility("hidden")));

extern void (*__fini_array_start[])(void) __attribute__((weak, visibility("hidden")));
extern void (*__fini_array_end[])(void) __attribute__((weak, visibility("hidden")));


/**
 * Current state of the shared library.
 **/
static int initialized = 0;


/**
 * Invoke .preinit and .init functions.
 **/
__attribute__((visibility("hidden"))) int
_init(void) {
  long count;

  if(initialized) {
    return 0;
  }
  initialized = 1;

  if(rtld_init()) {
    initialized = 0;
    return -1;
  }

  count = __preinit_array_end - __preinit_array_start;
  for(long i=0; i<count; i++) {
    __preinit_array_start[i]();
  }

  count = __init_array_end - __init_array_start;
  for(long i=0; i<count; i++) {
    __init_array_start[i]();
  }

  return 0;
}


/**
 * Invoke .fini functions.
 **/
__attribute__((visibility("hidden"))) int
_fini(void) {
  long count;

  if(!initialized) {
    return 0;
  }
  initialized = 0;

  count = __fini_array_end - __fini_array_start;
  for(long i=0; i<count; i++) {
    __fini_array_start[count-i-1]();
  }

  return 0;
}
