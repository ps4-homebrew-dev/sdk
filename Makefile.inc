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

MAKE   ?= gmake
PYTHON ?= python3

DESTDIR ?= /opt/ps4-homebrew-sdk

LLVM_CONFIG_CANDIDATES := llvm-config-23 llvm-config-22 llvm-config-21 \
                          llvm-config
LLVM_CONFIG_CANDIDATES := $(strip $(foreach LCC,$(LLVM_CONFIG_CANDIDATES), \
                          $(if $(shell command -v $(LCC) 2>/dev/null),$(LCC))))

LLVM_CONFIG ?= $(firstword $(LLVM_CONFIG_CANDIDATES))
ifeq ($(strip $(LLVM_CONFIG)),)
    $(error Unable to find llvm-config; set LLVM_CONFIG explicitly)
endif

LLVM_BINDIR := $(shell $(LLVM_CONFIG) --bindir)

CC  := $(LLVM_BINDIR)/clang
AR  := $(LLVM_BINDIR)/llvm-ar
LD  := $(LLVM_BINDIR)/ld.lld
