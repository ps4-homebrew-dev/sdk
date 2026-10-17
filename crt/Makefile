#   Copyright (C) 2026 John Törnblom
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING. If not see
# <http://www.gnu.org/licenses/>.

include ../Makefile.inc

CFLAGS := -target x86_64-scei-ps4 -std=c11 -O1
CFLAGS += -ffreestanding -fno-builtin -nostdlib -fPIC
CFLAGS += -fvisibility-nodllstorageclass=default
CFLAGS += -Wall -Werror

OBJS := crt1.o crti.o crtn.o crtbegin.o crtend.o crtbeginS.o crtendS.o

.PHONY: all clean install


all: $(OBJS)

crt1.o: crt1.c rtld.h
	$(CC) -c $(CFLAGS) -o $@ $<

crtbeginS.o: crtS.c rtld.h
	$(CC) -c $(CFLAGS) -o $@ $<

crtendS.o:
	$(AR) -rsc $@

crtbegin.o:
	$(AR) -rsc $@

crtend.o:
	$(AR) -rsc $@

crti.o:
	$(AR) -rsc $@

crtn.o:
	$(AR) -rsc $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o

install: $(OBJS)
	install -d $(DESTDIR)/target/lib
	install $(OBJS) $(DESTDIR)/target/lib
