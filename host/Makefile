# Copyright (C) 2026 John Törnblom
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

TOOLCHAINS := toolchain/orbis-clang.cfg \
              toolchain/orbis-clang++.cfg \
              toolchain/orbis.cmake \
              toolchain/orbis.ini \
              toolchain/orbis.mk \
              toolchain/orbis.sh

TOOLS := bin/orbis-clang \
         bin/orbis-clang++ \
         bin/orbis-cmake \
         bin/orbis-deploy \
         bin/orbis-elf2self \
         bin/orbis-mksfo \
         bin/orbis-mkgp4 \
         bin/orbis-mkpkg \
         bin/orbis-ld \
         bin/orbis-llvm \
         bin/orbis-llvm-config \
         bin/orbis-meson \
         bin/orbis-pkg-config

LDSCRIPTS := ldscripts/main.script \
             ldscripts/prx.script

.PHONY: all clean install

all:

clean:

install:
	install -d $(DESTDIR)/bin
	install -d $(DESTDIR)/toolchain
	install -d $(DESTDIR)/target/lib

	install -m 755 $(TOOLS)      $(DESTDIR)/bin
	install -m 644 $(TOOLCHAINS) $(DESTDIR)/toolchain
	install -m 644 $(LDSCRIPTS)  $(DESTDIR)/target/lib

	ln -sf orbis-llvm $(DESTDIR)/bin/llvm-ar
	ln -sf orbis-llvm $(DESTDIR)/bin/llvm-nm
	ln -sf orbis-llvm $(DESTDIR)/bin/llvm-objcopy
	ln -sf orbis-llvm $(DESTDIR)/bin/llvm-ranlib
	ln -sf orbis-llvm $(DESTDIR)/bin/llvm-strip
	ln -sf orbis-llvm $(DESTDIR)/bin/clang
	ln -sf orbis-llvm $(DESTDIR)/bin/clang++
	ln -sf orbis-llvm $(DESTDIR)/bin/clang-cpp
	ln -sf orbis-llvm $(DESTDIR)/bin/ld.lld
	ln -sf llvm-ar $(DESTDIR)/bin/orbis-ar
	ln -sf llvm-nm $(DESTDIR)/bin/orbis-nm
	ln -sf llvm-objcopy $(DESTDIR)/bin/orbis-objcopy
	ln -sf llvm-ranlib $(DESTDIR)/bin/orbis-ranlib
	ln -sf llvm-strip $(DESTDIR)/bin/orbis-strip
	ln -sf orbis-clang $(DESTDIR)/bin/orbis-as
