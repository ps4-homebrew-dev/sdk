#!/usr/bin/env bash
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

PS4_HOMEBREW_SDK="${BASH_SOURCE[0]}"
PS4_HOMEBREW_SDK="$(dirname "${PS4_HOMEBREW_SDK}")"
PS4_HOMEBREW_SDK="${PS4_HOMEBREW_SDK}/.."
PS4_HOMEBREW_SDK="$(realpath "${PS4_HOMEBREW_SDK}")"

export PS4_HOMEBREW_SDK=${PS4_HOMEBREW_SDK}

export SYSROOT="${PS4_HOMEBREW_SDK}/target"
export PREFIX="/usr"

export AS=${PS4_HOMEBREW_SDK}/bin/orbis-as
export CC=${PS4_HOMEBREW_SDK}/bin/orbis-clang
export CXX=${PS4_HOMEBREW_SDK}/bin/orbis-clang++
export OBJC=${PS4_HOMEBREW_SDK}/bin/orbis-clang
export OBJCXX=${PS4_HOMEBREW_SDK}/bin/orbis-clang++
export LD=${PS4_HOMEBREW_SDK}/bin/orbis-ld

export AR=${PS4_HOMEBREW_SDK}/bin/orbis-ar
export NM=${PS4_HOMEBREW_SDK}/bin/orbis-nm
export OBJCOPY=${PS4_HOMEBREW_SDK}/bin/orbis-objcopy
export RANLIB=${PS4_HOMEBREW_SDK}/bin/orbis-ranlib
export STRIP=${PS4_HOMEBREW_SDK}/bin/orbis-strip

export MAKE=gmake
export CMAKE=${PS4_HOMEBREW_SDK}/bin/orbis-cmake
export MESON=${PS4_HOMEBREW_SDK}/bin/orbis-meson
export PKG_CONFIG=${PS4_HOMEBREW_SDK}/bin/orbis-pkg-config

export ELF2SELF=${PS4_HOMEBREW_SDK}/bin/orbis-elf2self
export MKSFO=${PS4_HOMEBREW_SDK}/bin/orbis-mksfo
export MKGP4=${PS4_HOMEBREW_SDK}/bin/orbis-mkgp4
export MKPKG=${PS4_HOMEBREW_SDK}/bin/orbis-mkpkg
export DEPLOY=${PS4_HOMEBREW_SDK}/bin/orbis-deploy
