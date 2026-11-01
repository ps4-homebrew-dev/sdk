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

set(PS4_HOMEBREW_SDK "${CMAKE_CURRENT_LIST_DIR}/.." CACHE PATH "")
set(PS4 TRUE)
set(ORBIS TRUE)

#
# The PS4 is running a modified FreeBSD kernel on a x86_64 CPU
#
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_SYSTEM_NAME FreeBSD)
set(CMAKE_SYSTEM_VERSION 9)
set(CMAKE_CROSSCOMPILING 1)

#
# Set sysroot and search paths
#
set(CMAKE_SYSROOT "${PS4_HOMEBREW_SDK}/target")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#
# Set prefix (on the target platform)
#
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "")

#
# Encrease default verbosity in makefiles
#
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "")

#
# We only support positional independent executables
#
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Shared libs turned off by default")
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "")

#
# Set external tool dependencies
#
set(CMAKE_ASM_COMPILER ${PS4_HOMEBREW_SDK}/bin/orbis-as CACHE PATH "")
set(CMAKE_C_COMPILER ${PS4_HOMEBREW_SDK}/bin/orbis-clang CACHE PATH "")
set(CMAKE_CXX_COMPILER ${PS4_HOMEBREW_SDK}/bin/orbis-clang++ CACHE PATH "")
set(CMAKE_LINKER ${PS4_HOMEBREW_SDK}/bin/orbis-ld CACHE PATH "")

set(CMAKE_AR ${PS4_HOMEBREW_SDK}/bin/orbis-ar CACHE PATH "")
set(CMAKE_C_COMPILER_AR ${PS4_HOMEBREW_SDK}/bin/orbis-ar CACHE PATH "")
set(CMAKE_CXX_COMPILER_AR ${PS4_HOMEBREW_SDK}/bin/orbis-ar CACHE PATH "")

set(CMAKE_RANLIB ${PS4_HOMEBREW_SDK}/bin/orbis-ranlib CACHE PATH "")
set(CMAKE_C_COMPILER_RANLIB ${PS4_HOMEBREW_SDK}/bin/orbis-ranlib CACHE PATH "")
set(CMAKE_CXX_COMPILER_RANLIB ${PS4_HOMEBREW_SDK}/bin/orbis-ranlib CACHE PATH "")

set(CMAKE_STRIP ${PS4_HOMEBREW_SDK}/bin/orbis-strip CACHE PATH "")

set(PKG_CONFIG_EXECUTABLE ${PS4_HOMEBREW_SDK}/bin/orbis-pkg-config CACHE PATH "")
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

set(DEPLOY ${PS4_HOMEBREW_SDK}/bin/orbis-deploy CACHE PATH "")
set(ELF2SELF ${PS4_HOMEBREW_SDK}/bin/orbis-elf2self CACHE PATH "")
set(MKSFO ${PS4_HOMEBREW_SDK}/bin/orbis-mksfo CACHE PATH "")
set(MKGP4 ${PS4_HOMEBREW_SDK}/bin/orbis-mkgp4 CACHE PATH "")
set(MKPKG ${PS4_HOMEBREW_SDK}/bin/orbis-mkpkg CACHE PATH "")

function(orbis_add_self target)
  if(ARGC GREATER 2)
    message(FATAL_ERROR "orbis_add_self: unknown arguments: ${ARGN}")
  endif()
  set(out "$<TARGET_FILE_NAME:${target}>.self")
  if(ARGC EQUAL 2)
    set(out "${ARGV1}")
  endif()
  if(NOT IS_ABSOLUTE "${out}")
    set(out "$<TARGET_FILE_DIR:${target}>/${out}")
  endif()
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${ELF2SELF} $<TARGET_FILE:${target}> ${out}
    VERBATIM)
  set_property(TARGET ${target} APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${out})
endfunction()

function(orbis_add_pkg target)
  cmake_parse_arguments(PARSE_ARGV 1 ARG ""
    "TITLE;TITLE_ID;CONTENT_ID;VERSION;CATEGORY;APP_TYPE;ATTRIBUTE;SYSTEM_VER;DOWNLOAD_DATA_SIZE"
    "ASSETS;MODULES")

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "orbis_add_pkg: unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  if(NOT ARG_TITLE OR NOT ARG_TITLE_ID)
    message(FATAL_ERROR "orbis_add_pkg: TITLE and TITLE_ID are required")
  endif()
  foreach(default VERSION=1.00 CATEGORY=gd APP_TYPE=1 ATTRIBUTE=2
                  SYSTEM_VER=0 DOWNLOAD_DATA_SIZE=0)
    string(REPLACE "=" ";" default ${default})
    list(GET default 0 key)
    if(NOT DEFINED ARG_${key})
      list(GET default 1 ARG_${key})
    endif()
  endforeach()
  if(NOT ARG_CONTENT_ID)
    string(TOUPPER "${ARG_TITLE}0000000000000000" label)
    string(REGEX REPLACE "[^A-Z0-9]" "" label "${label}")
    string(SUBSTRING "${label}" 0 16 label)
    set(ARG_CONTENT_ID "IV0000-${ARG_TITLE_ID}_00-${label}")
  endif()

  set(app0 ${CMAKE_CURRENT_BINARY_DIR}/${target}_app0)
  set(pkg ${CMAKE_CURRENT_BINARY_DIR}/${ARG_CONTENT_ID}.pkg)
  set(files eboot.bin sce_sys/param.sfo)

  add_custom_command(OUTPUT ${app0}/eboot.bin
    COMMAND ${CMAKE_COMMAND} -E make_directory ${app0}
    COMMAND ${ELF2SELF} $<TARGET_FILE:${target}> ${app0}/eboot.bin
    DEPENDS ${target} VERBATIM)

  add_custom_command(OUTPUT ${app0}/sce_sys/param.sfo
    COMMAND ${CMAKE_COMMAND} -E make_directory ${app0}/sce_sys
    COMMAND ${MKSFO} -i APP_TYPE=${ARG_APP_TYPE}
                     -i ATTRIBUTE=${ARG_ATTRIBUTE}
                     -i SYSTEM_VER=${ARG_SYSTEM_VER}
                     -i DOWNLOAD_DATA_SIZE=${ARG_DOWNLOAD_DATA_SIZE}
                     -s4 CATEGORY=${ARG_CATEGORY}
                     -s128 TITLE=${ARG_TITLE}
                     -s12 TITLE_ID=${ARG_TITLE_ID}
                     -s48 CONTENT_ID=${ARG_CONTENT_ID}
                     -s8 APP_VER=${ARG_VERSION}
                     -s8 VERSION=${ARG_VERSION}
                     ${app0}/sce_sys/param.sfo
    VERBATIM)

  foreach(module ${ARG_MODULES})
    get_filename_component(src ${module} ABSOLUTE
                           BASE_DIR ${CMAKE_SYSROOT}/usr/lib)
    get_filename_component(name ${src} NAME)
    add_custom_command(OUTPUT ${app0}/sce_module/${name}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${app0}/sce_module
      COMMAND ${ELF2SELF} ${src} ${app0}/sce_module/${name}
      DEPENDS ${src} VERBATIM)
    list(APPEND files sce_module/${name})
  endforeach()

  foreach(asset ${ARG_ASSETS})
    get_filename_component(asset ${asset} ABSOLUTE)
    if(IS_DIRECTORY ${asset})
      file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ${asset}/*)
      # Skip hidden files and editor backups.
      list(FILTER srcs EXCLUDE REGEX "/\\.[^/]*$|~$")
      list(SORT srcs)
    else()
      set(srcs ${asset})
    endif()
    foreach(src ${srcs})
      file(RELATIVE_PATH dst ${CMAKE_CURRENT_SOURCE_DIR} ${src})
      add_custom_command(OUTPUT ${app0}/${dst}
        COMMAND ${CMAKE_COMMAND} -E copy ${src} ${app0}/${dst}
        DEPENDS ${src} VERBATIM)
      list(APPEND files ${dst})
    endforeach()
  endforeach()

  list(TRANSFORM files PREPEND ${app0}/ OUTPUT_VARIABLE deps)
  add_custom_command(OUTPUT ${pkg}
    COMMAND ${MKGP4} -c ${ARG_CONTENT_ID} -o ${target}.gp4 ${files}
    COMMAND ${MKPKG} -o ${pkg} ${target}.gp4
    WORKING_DIRECTORY ${app0}
    DEPENDS ${deps} VERBATIM)
  add_custom_target(${target}_pkg ALL DEPENDS ${pkg})
endfunction()
