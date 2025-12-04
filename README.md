## Welcome to Small-LibC
Small-LibC is an iOS development library, which implements some of standart C functions

## Get started
If you're on iOS, just run `./make_all.sh`, then you'll be able to use it

If you're cross-compiling, go to ./src, then `make CROSS=1`. Supported Makefile flags are `SDK`, `ARCHES` (armv7 default, armv4t-7s permitted), `CFLAGS`, `CPPFLAGS`. If you have cctools, use Makefile flag `LLVM-PREFIX=""`. To turn off PIC use `PIC=""`

## Status
About 15% of C89 is implemented. Anyway, some funtions from stdlib.h and unistd.h are done. Also, string.h is done, but it's git submodule. Run `git submodule update --recursive --init`

## License
All of my code licensed under MIT License

## Structure

1. ./apple – Apple's code under APSL, C Start Up code
2. ./src – LibC's source
3. ./include – LibC's headers (stdint.h licensed under APSL)
4. ./src/sysindep – Probably system-independent code
5. ./src/sysdep (see below)
6. ./test – unittests

## Portage
To port this one, you just need to make `./src/sysdep/*/*` files and replace `./include/sys/*` (and some other headers). Simplest way is to export symbols from gLibC
