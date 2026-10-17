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


#include "rtld.h"


/**
 * Prototypes for data structures used by libSceLibcInternal.sprx.
 **/
typedef struct __attribute__((packed)) {
  unsigned long size;
  unsigned int entry_count;
  unsigned int internal_heap;
  unsigned int *heap_size;
  unsigned int *heap_delayed_alloc;
  unsigned int *heap_extended_alloc;
  unsigned int *heap_initial_size;
  void* malloc_replace;
  void* new_replace;
  unsigned long *heap_high_address_alloc;
  unsigned int *need_libc;
  unsigned long *heap_memory_lock;
  unsigned long *internal_memory_size;
  void* malloc_replace_for_tls;
  unsigned long *max_system_size;
  unsigned long *heap_debug_flags;
  unsigned int *std_thread_stack_size;
  unsigned long unknown;
  unsigned int *internal_memory_debug_flags;
  unsigned int *worker_thread_num;
  unsigned int *worker_thread_priority;
  unsigned int *thread_unnamed_objects;
} libc_param_t;

typedef struct __attribute__((packed)) {
  unsigned long size;
  unsigned long *extended_page_table;
  unsigned long *flexible_memory_size;
  unsigned char *extended_memory1;
  unsigned long *extended_gpu_page_table;
  unsigned char *extended_memory2;
  unsigned long *extended_cpu_page_table;
} mem_param_t;

typedef struct __attribute__((packed)) {
  unsigned long size;
  unsigned int *fs_dup_dent;
  void* workspace_update_mode;
  void* trace_apr_name_mode;
} fs_param_t;


/**
 * Prototype for data structure used by the kernel when starting a new process.
 **/
typedef struct __attribute__((packed)) {
  unsigned long size;
  unsigned int magic;
  unsigned int entry_count;
  unsigned long sdk_version;
  const char *process_name;
  const char *thread_name;
  unsigned int *thread_priority;
  unsigned int *thread_stack_size;
  libc_param_t *libc_param;
  mem_param_t *mem_param;
  fs_param_t *fs_param;
  unsigned int *preload_enabled;
  unsigned long flags;
} process_param_t;


/**
 * Parameters used by libSceLibcInternal.sprx.
 **/
static unsigned int heap_size = 0xffffffff;
static unsigned int heap_extended_alloc = 1;

__attribute__((used, aligned(8)))
static libc_param_t libc_param = {
  .size = sizeof(libc_param_t),
  .entry_count = 0x0e,
  .internal_heap = 0,
  .heap_size = &heap_size,
  .heap_extended_alloc = &heap_extended_alloc
};


/**
 * Parameters used by the kernel used when starting a new process.
 **/
__attribute__((used, section(".sce_process_param"), aligned(8)))
static process_param_t process_param = {
  .size = sizeof(process_param_t),
  .magic = 0x4942524F,
  .entry_count = 1,
  .libc_param = &libc_param,
};


/**
 * Dependencies provided by the ELF linker.
 **/
extern void (*__preinit_array_start[])(void) __attribute__((weak));
extern void (*__preinit_array_end[])(void) __attribute__((weak));

extern void (*__init_array_start[])(void) __attribute__((weak));
extern void (*__init_array_end[])(void) __attribute__((weak));

extern void (*__fini_array_start[])(void) __attribute__((weak));
extern void (*__fini_array_end[])(void) __attribute__((weak));


/**
 * Dependencies provided by libSceLibcInternal.sprx
 **/
extern void _init_env(void*);
extern int atexit(void (*fn)(void));
extern void exit(int status);


/**
 * Dependencies provided by the homebrew.
 **/
extern int main(int argc, char **argv, char **envp);


/**
 * Homebrew can override this to catch exit status codes.
 **/
__attribute__((weak)) void
catchReturnFromMain(int exit_status) {
  return;
}


/**
 * Invoke .preinit and .init functions.
 **/
void
_init(void) {
  long count;

  count = __preinit_array_end - __preinit_array_start;
  for(int i=0; i<count; i++) {
    __preinit_array_start[i]();
  }

  count = __init_array_end - __init_array_start;
  for(int i=0; i<count; i++) {
    __init_array_start[i]();
  }
}


/**
 * Invoke .fini functions.
 **/
void
_fini(void) {
  long count = __fini_array_end - __fini_array_start;
  for(int i=0; i<count; i++) {
    __fini_array_start[count-i-1]();
  }
}


/**
 * Entry point to the homebrew ELF image.
 **/
__attribute__((noreturn, used, force_align_arg_pointer, no_stack_protector)) void
_start(long *ap, void (*cleanup)(void)) {
  char **argv = (char **)&ap[1];
  int argc = ap[0];
  int exit_code;

  exit_code = rtld_init();

  atexit(cleanup);
  atexit(_fini);

  _init_env(ap);
  _init();

  if(!exit_code) {
    exit_code = main(argc, argv, argv + argc + 1);
    catchReturnFromMain(exit_code);
  }

  exit(exit_code);
  __builtin_trap();
}
