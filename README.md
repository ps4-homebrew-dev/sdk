# ps4-homebrew-dev SDK

A toolchain for writing homebrew for jailbroken PS4 consoles (and for
running PS4 homebrew on a jailbroken PS5) with a stock LLVM/Clang. It
provides:

- `orbis-clang`, `orbis-clang++`, `orbis-ld` and the other `orbis-*`
  wrappers, which build FreeBSD-style ELF executables for the console
- C runtime start files (`crt/`), a small libc on top of the system's
  (`libc/`), and stubs for the system libraries (`stubs/`)
- the headers (`include/`)
- `orbis-elf2self`, `orbis-mksfo`, `orbis-mkgp4` and `orbis-mkpkg` to make
  signed executables (`.self`) and installable packages (`.pkg`)
- `orbis-cmake`, `orbis-meson` and `orbis-pkg-config`, set up to
  cross-compile, with the CMake functions `orbis_add_self()` and
  `orbis_add_pkg()`
- `orbis-deploy`, which sends a `.self` to an ELF loader on the console

Most homebrew also needs libraries (SDL2, Mesa, mpv, ...). Those are built
with this SDK in [pacbrew](https://github.com/ps4-homebrew-dev/pacbrew),
whose release bundles the SDK and the libraries together; that is the
easiest way to get both.

## Installing
The prebuilt SDK, with the libraries:

```sh
wget https://github.com/ps4-homebrew-dev/pacbrew/releases/latest/download/ps4-homebrew-dev.tar.gz
sudo tar xf ps4-homebrew-dev.tar.gz -C /
```

Or the SDK alone, from source. It needs LLVM, Clang and LLD (version 20 or
later), GNU make, Python 3, and CMake and Meson for those wrappers:

```sh
sudo apt install llvm-20 clang-20 lld-20 cmake meson
make
sudo make install    # DESTDIR=/opt/ps4-homebrew-sdk by default
```

Set `LLVM_CONFIG` if the right `llvm-config` is not found on its own.

## Using
Source `toolchain/orbis.sh` to get `CC`, `CXX`, `CMAKE`, `MESON`,
`PKG_CONFIG`, `ELF2SELF` and the rest in the environment:

```sh
source /opt/ps4-homebrew-sdk/toolchain/orbis.sh
cp -r /opt/ps4-homebrew-sdk/samples/hello_world .
make -C hello_world    # hello_world.self
```

With CMake, `orbis-cmake` brings the toolchain file along:

```cmake
add_executable(hello main.c)
orbis_add_self(hello)                 # hello.self, to run with orbis-deploy
orbis_add_pkg(hello                   # hello.pkg, to install
    TITLE "Hello"
    TITLE_ID BREW00001
    CONTENT_ID IV0000-BREW00001_00-HELLO00000000000
    ASSETS sce_sys)                   # icon0.png, pic0.png, ...
```

```sh
/opt/ps4-homebrew-sdk/bin/orbis-cmake -S . -B build
make -C build
```

`orbis_add_pkg()` also takes `VERSION`, `CATEGORY` (`gd` for a game, the
default, or `gdc` for a non-game application), `APP_TYPE`, `ATTRIBUTE`
(`8454146` asks for PS4 Pro mode with seven CPUs), `SYSTEM_VER`,
`DOWNLOAD_DATA_SIZE` (MiB, for `/download0`) and `MODULES` (shared
libraries to put in the package).

To run a `.self` straight away, start an ELF loader on the console and:

```sh
orbis-deploy -h <console address> hello.self    # port 9021 by default
```

The samples in `samples/` show plain make, CMake, Meson, `dlopen()` and
linking against a system module.

## License
GPL-3.0-or-later, see [LICENSE](LICENSE). The headers in `include/` come
from FreeBSD and other projects, under their own licenses.
