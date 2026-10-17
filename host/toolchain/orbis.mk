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

PS4_HOMEBREW_SDK := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))..)

AS         := $(PS4_HOMEBREW_SDK)/bin/orbis-clang
CC         := $(PS4_HOMEBREW_SDK)/bin/orbis-clang
CXX        := $(PS4_HOMEBREW_SDK)/bin/orbis-clang++
LD         := $(PS4_HOMEBREW_SDK)/bin/orbis-ld
PKG_CONFIG := $(PS4_HOMEBREW_SDK)/bin/orbis-pkg-config
AR         := $(PS4_HOMEBREW_SDK)/bin/orbis-ar
NM         := $(PS4_HOMEBREW_SDK)/bin/orbis-nm
OBJCOPY    := $(PS4_HOMEBREW_SDK)/bin/orbis-objcopy
RANLIB     := $(PS4_HOMEBREW_SDK)/bin/orbis-ranlib
STRIP      := $(PS4_HOMEBREW_SDK)/bin/orbis-strip

ELF2SELF   := $(PS4_HOMEBREW_SDK)/bin/orbis-elf2self
MKSFO      := $(PS4_HOMEBREW_SDK)/bin/orbis-mksfo
MKGP4      := $(PS4_HOMEBREW_SDK)/bin/orbis-mkgp4
MKPKG      := $(PS4_HOMEBREW_SDK)/bin/orbis-mkpkg
DEPLOY     := $(PS4_HOMEBREW_SDK)/bin/orbis-deploy

SYSROOT    := $(PS4_HOMEBREW_SDK)/target
PREFIX     := /usr
