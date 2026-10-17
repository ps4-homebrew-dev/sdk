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

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>


const char* sceKernelGetFsSandboxRandomWord(void);


FILE*
tmpfile(void) {
  struct stat st;
  char path[100];
  FILE* f;
  int fd;

  if(!stat("/user", &st)) {
    strcpy(path, "/user/temp/common_temp/tmpfile_XXXXXX");
  } else {
    sprintf(path, "/%s/common_temp/tmpfile_XXXXXX", sceKernelGetFsSandboxRandomWord());
  }

  if((fd=mkstemp(path)) < 0) {
    return 0;
  }

  unlink(path);

  if(!(f=fdopen(fd, "rw+b"))) {
    close(fd);
  }

  return f;
}
