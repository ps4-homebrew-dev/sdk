/* Copyright (C) 2024 John Törnblom

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>


const char* sceKernelGetFsSandboxRandomWord(void);


char*
tmpnam(char* s) {
  static char buf[L_tmpnam > 100 ? L_tmpnam : 100];
  struct stat st;
  char path[100];
  int fd;

  // same lookup strategy as tmpfile()
  if(!stat("/user", &st)) {
    strcpy(path, "/user/temp/common_temp/tmpfile_XXXXXX");
  } else {
    snprintf(path, sizeof(path), "/%s/common_temp/tmpfile_XXXXXX",
	     sceKernelGetFsSandboxRandomWord());
  }

  if((fd=mkstemp(path)) < 0) {
    return 0;
  }

  close(fd);
  unlink(path);

  if(!s) {
    s = buf;
  }

  strcpy(s, path);

  return s;
}
