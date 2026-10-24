#!/usr/bin/env python3
# encoding: utf-8
# Copyright (C) 2025 John Törnblom
#
# This program is free software; you can redistribute it and/or modify it
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

import argparse
import os
import string
import struct

from elftools.elf.elffile import ELFFile


tmpl_func = string.Template('''
asm(".global $name\\n"
    ".type $name @function\\n"
    "$name:\\n");''')


tmpl_obj = string.Template('''
asm(".global $name\\n"
    ".type   $name, @object\\n"
    "$name:\\n");''')


# read NIDs from aerolib.csv
NID_DB = (os.path.dirname(__file__) or '.') + '/aerolib.csv'
nid_map = {}
with open(NID_DB) as f:
    for line in f.readlines():
        nid, sym = line.strip().split(' ', 1)
        nid_map[nid] = sym


ID_ALPHABET = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-'


def decode_id(text):
    '''
    decode a module or library id from a symbol name (NID#<lib>#<mod>),
    in base 64: "A" is 0, "B" is 1, "BA" is 64.
    '''
    value = 0
    for c in text:
        value = value * 64 + ID_ALPHABET.index(c)
    return value


def read_cstr(f, offset):
    f.seek(offset)
    data = b''
    while True:
        chunk = f.read(64)
        if not chunk:
            break
        if b'\0' in chunk:
            data += chunk[:chunk.index(b'\0')]
            break
        data += chunk
    return data.decode('ascii')


def symbols(sym_type, filename):
    '''
    yield (name, version) of the symbols in dynamic segments, named using the
    NID lookup table 'aerolib.csv'.  The version is "<module>:<library>": the
    name of the module, from DT_SCE_MODULE_INFO, and that of the library that
    exports the symbol, from DT_SCE_EXPORT_LIB.  These often differ from the
    file name.
    Based on:
    https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain/blob/master/scripts/symbol_entries.py
    '''
    with open(filename, 'rb') as f:
        elf = ELFFile(f)

        dynamic = None
        dynlibdata = None
        for segment in elf.iter_segments():
            if segment.header.p_type == 'PT_DYNAMIC':
                dynamic = segment.header

            elif segment.header.p_type == 0x61000000: # PT_SCE_DYNLIBDATA
                dynlibdata = segment.header

        if not dynamic:
            return

        dyncnt = int(dynamic.p_filesz / struct.calcsize('<QQ'))
        symtab = strtab = dynlibdata.p_offset
        symcnt = 0
        module_info = None
        export_libs = []

        for i in range(dyncnt):
            f.seek(dynamic.p_offset + (i * struct.calcsize('<QQ')))
            d_tag, d_val = struct.unpack('<QQ', f.read(struct.calcsize('<QQ')))

            if d_tag == 0x61000039: # DT_SCE_SYMTAB
                symtab += d_val

            if d_tag == 0x61000035: # DT_SCE_STRTAB
                strtab += d_val

            if d_tag == 0x6100003F: # DT_SCE_SYMTABSZ
                symcnt = int(d_val / struct.calcsize('<IBBHQQ'))

            if d_tag == 0x6100000D: # DT_SCE_MODULE_INFO
                module_info = d_val

            if d_tag == 0x61000013: # DT_SCE_EXPORT_LIB
                export_libs.append(d_val)

        # Name offset in the low 32 bits, id in the high 16.
        module = None
        if module_info is not None:
            module = read_cstr(f, strtab + (module_info & 0xFFFFFFFF))
        libs = {val >> 48: read_cstr(f, strtab + (val & 0xFFFFFFFF))
                for val in export_libs}

        for i in range(symcnt):
            f.seek(symtab + (i * struct.calcsize('<IBBHQQ')))
            st_name, st_info, st_other, st_shndx, st_value, st_size = \
                struct.unpack('<IBBHQQ', f.read(struct.calcsize('<IBBHQQ')))

            st_bind = st_info >> 4
            st_type = st_info & 0xF

            if st_shndx == 0: # SHN_UNDEF
                continue

            if st_bind == 0: # STB_LOCAL
                continue

            if sym_type == 'STT_OBJECT' and st_type != 1:
                continue

            if sym_type == 'STT_FUNC' and st_type != 2:
                continue

            symname = read_cstr(f, strtab + st_name)
            if symname.count('#') != 2:
                continue
            nid, lid, mid = symname.split('#')

            if nid not in nid_map:
                continue

            version = None
            lib = libs.get(decode_id(lid))
            if module and lib:
                version = module + ':' + lib

            yield nid_map[nid], version


def versioned(syms):
    '''
    map each symbol name to one version.  A name that several of the module's
    libraries export gets the library named like the module, if that is one of
    them, and else the first one by name, so that the choice is stable.
    '''
    candidates = {}
    for name, version in syms:
        candidates.setdefault(name, set()).add(version)

    result = {}
    for name, versions in candidates.items():
        versions = sorted(v for v in versions if v)
        preferred = [v for v in versions if v.split(':')[0] == v.split(':')[1]]
        result[name] = (preferred or versions or [None])[0]
    return result


def version_script(syms):
    '''
    a version script for ld.lld that gives each symbol its version.
    orbis-elf2self reads the versions back from the stub to name the module
    and library each symbol is imported from.
    '''
    nodes = {}
    for name, version in sorted(syms.items()):
        if version:
            nodes.setdefault(version, []).append(name)

    out = []
    for version in sorted(nodes):
        out.append(version + ' {')
        out.append('  global:')
        out.extend('    ' + name + ';' for name in nodes[version])
        out.append('};')
        out.append('')
    return '\n'.join(out)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--version-script', action='store_true',
                        help='print a version script for ld.lld, naming the '
                        'module and library of each symbol, instead of C')
    parser.add_argument('SPRX_FILE')
    cli_args = parser.parse_args()

    funcs = versioned(symbols('STT_FUNC', cli_args.SPRX_FILE))
    gvars = versioned(symbols('STT_OBJECT', cli_args.SPRX_FILE))

    if cli_args.version_script:
        print(version_script({**gvars, **funcs}), end='')
    else:
        for name in sorted(funcs):
            print(tmpl_func.substitute(name=name))

        for name in sorted(gvars):
            print(tmpl_obj.substitute(name=name))
