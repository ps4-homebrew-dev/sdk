# ps4-homebrew-dev SDK

An SDK for making homebrew for jailbroken PS4 (and PS5) consoles, using
a stock LLVM/Clang toolchain.

## Install
The easiest way is to grab the prebuilt SDK, bundled with a bunch of
libraries (SDL2, Mesa, mpv, ...), from
[pacbrew](https://github.com/ps4-homebrew-dev/pacbrew):
```sh
wget https://github.com/ps4-homebrew-dev/pacbrew/releases/latest/download/ps4-homebrew-dev.tar.gz
sudo tar xf ps4-homebrew-dev.tar.gz -C /
```

To build the SDK yourself, you'll need LLVM, Clang and LLD 20 or later.
Then, just run:
```sh
make
sudo make install
```

## Usage
Have a look at the [samples](samples). With CMake, for example:
```sh
/opt/ps4-homebrew-sdk/bin/orbis-cmake -S samples/hello_cmake -B build
make -C build
```

## License
GPL-3.0-or-later, see [LICENSE](LICENSE).
