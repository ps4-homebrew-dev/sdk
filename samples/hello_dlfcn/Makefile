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

PS4_HOST ?= ps4
PS4_PORT ?= 9021

ifdef PS4_HOMEBREW_SDK
    include $(PS4_HOMEBREW_SDK)/toolchain/orbis.mk
else
    $(error PS4_HOMEBREW_SDK is undefined)
endif

CFLAGS := -Wall -Werror -g

all: hello_dlfcn.self

hello_dlfcn.elf: main.c
	$(CC) $(CFLAGS) -o $@ $^

hello_dlfcn.self: hello_dlfcn.elf
	$(ELF2SELF)  $^ $@

clean:
	rm -f *.o *.elf *.self

test: hello_dlfcn.self
	$(DEPLOY) -h $(PS4_HOST) -p $(PS4_PORT) $^
