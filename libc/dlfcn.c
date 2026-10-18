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

#include <dlfcn.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>


/**
 *
 **/
#define SYS_dynlib_dlsym       591
#define SYS_dynlib_get_list    592
#define SYS_dynlib_get_info_ex 608


/**
 *
 **/
const char* sceKernelGetFsSandboxRandomWord(void);
int sceKernelLoadStartModule(const char* path, size_t argc, const void* argv,
                             uint32_t flags, void* opt, int* res);
int sceKernelStopUnloadModule(int handle, size_t argc, const void* argv,
                              uint32_t flags, void* opt, int* res);


/**
 * The most loaded modules dladdr() looks at.
 **/
#define MAX_MODULES 512


/**
 *
 **/
typedef struct dynlib_segment {
  uint64_t addr;
  uint32_t size;
  uint32_t prot;
} dynlib_segment_t;


/**
 *
 **/
typedef struct dynlib_info_ex {
  uint64_t size;
  char name[256];
  int32_t handle;
  uint32_t tls_index;
  uint64_t tls_init_addr;
  uint32_t tls_init_size;
  uint32_t tls_size;
  uint32_t tls_offset;
  uint32_t tls_align;
  uint64_t init_proc_addr;
  uint64_t fini_proc_addr;
  uint64_t reserved1;
  uint64_t reserved2;
  uint64_t eh_frame_hdr_addr;
  uint64_t eh_frame_addr;
  uint32_t eh_frame_hdr_size;
  uint32_t eh_frame_size;
  dynlib_segment_t segments[4];
  uint32_t segment_count;
  uint32_t ref_count;
} dynlib_info_ex_t;


/**
 *
 **/
static _Thread_local char g_error[512];
static _Thread_local int g_error_pending = 0;


/**
 *
 **/
static void
seterror_errno(const char* subject, int errnum) {
  char reason[128];

  if(!subject) {
    subject = "(null)";
  }

  if(strerror_r(errnum, reason, sizeof reason)) {
    snprintf(reason, sizeof reason, "Unknown error %d", errnum);
  }
  snprintf(g_error, sizeof g_error, "%s: %s", subject, reason);
  g_error_pending = 1;
}


/**
 *
 **/
static void
seterror(const char* message) {
  snprintf(g_error, sizeof g_error, "%s", message);
  g_error_pending = 1;
}


/**
 *
 **/
static int
dlopen_resolve(const char* name, char* path, size_t size) {
  static const char* const system_dirs[] = {
    "/system/priv/lib",
    "/system/common/lib",
    "/system_ex/priv_ex/lib",
    "/system_ex/common_ex/lib",
  };
  static const char* const sandbox_dirs[] = {
    "priv/lib",
    "common/lib",
    "priv_ex/lib",
    "common_ex/lib",
  };
  const char* random_word;
  struct stat st;

  for(int i=0; i<sizeof(system_dirs)/sizeof(system_dirs[0]); i++) {
    snprintf(path, size, "%s/%s", system_dirs[i], name);
    if(!stat(path, &st)) {
      return 0;
    }
  }

  if((random_word=sceKernelGetFsSandboxRandomWord())) {
    for(int i=0; i<sizeof(sandbox_dirs)/sizeof(sandbox_dirs[0]); i++) {
      snprintf(path, size, "/%s/%s/%s", random_word, sandbox_dirs[i], name);
      if(!stat(path, &st)) {
        return 0;
      }
    }
  }

  snprintf(path, size, "/app0/sce_module/%s", name);
  if(!stat(path, &st)) {
    return 0;
  }

  snprintf(path, size, "%s", name);
  if(!stat(path, &st)) {
    return 0;
  }

  return -1;
}


void*
dlopen(const char* path, int flags) {
  char resolved[PATH_MAX];
  int handle;

  if(!path) {
    seterror_errno("dlopen", EINVAL);
    return 0;
  }

  if(dlopen_resolve(path, resolved, sizeof(resolved))) {
    seterror_errno(path, ENOENT);
    return 0;
  }

  if((handle=sceKernelLoadStartModule(resolved, 0, 0, 0, 0, 0)) < 0) {
    seterror_errno(path, handle & 0xffff);
    return 0;
  }

  return (void*)(intptr_t)handle;
}


int
dlclose(void* handle) {
  int err;

  if((err=sceKernelStopUnloadModule((int)(intptr_t)handle, 0, 0, 0, 0, 0)) < 0) {
    seterror_errno("dlclose", err & 0xffff);
    return -1;
  }

  return 0;
}


void*
dlsym(void* handle, const char* symbol) {
  void* addr = 0;

  if(syscall(SYS_dynlib_dlsym, handle, symbol, &addr) < 0) {
    seterror_errno(symbol, errno);
    return 0;
  }

  return addr;
}


char*
dlerror(void) {
  if(!g_error_pending) {
    return 0;
  }

  g_error_pending = 0;
  return g_error;
}


int
dladdr(const void* paddr, Dl_info* info) {
  static _Thread_local char fname[256];
  uintptr_t addr = (uintptr_t)paddr;
  int handles[MAX_MODULES];
  dynlib_info_ex_t mod;
  size_t count = 0;
  uintptr_t base;
  int found;

  if(syscall(SYS_dynlib_get_list, handles, MAX_MODULES, &count) < 0) {
    seterror_errno("dladdr", errno);
    return 0;
  }
  if(count > MAX_MODULES) {
    count = MAX_MODULES;
  }

  for(size_t i=0; i<count; i++) {
    memset(&mod, 0, sizeof(mod));
    mod.size = sizeof(mod);

    if(syscall(SYS_dynlib_get_info_ex, handles[i], 0, &mod) < 0) {
      continue;
    }
    if(mod.segment_count > 4) {
      mod.segment_count = 4;
    }

    // Look at every segment before deciding: the module's base address is
    // its lowest segment, which need not be the one containing addr.
    base = UINTPTR_MAX;
    found = 0;
    for(uint32_t j=0; j<mod.segment_count; j++) {
      if(!mod.segments[j].size) {
        continue;
      }
      if(mod.segments[j].addr < base) {
        base = mod.segments[j].addr;
      }
      if(addr >= mod.segments[j].addr &&
         addr - mod.segments[j].addr < mod.segments[j].size) {
        found = 1;
      }
    }

    if(found) {
      strncpy(fname, mod.name, sizeof(fname) - 1);
      fname[sizeof(fname) - 1] = 0;
      info->dli_fname = fname;
      info->dli_fbase = (void*)base;
      info->dli_sname = 0;
      info->dli_saddr = 0;

      return 1;
    }
  }

  seterror("dladdr: No loaded module contains the address");
  return 0;
}
