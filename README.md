## Welcome to Small-LibC
Small-LibC is an iOS development library, which implements some of standart C functions

## Get started
If you're on iOS, just run `./make_all.sh`, then you'll be able to use it as libc.a (recommended way)

If you're cross-compiling, go to ./src, then `make CROSS=1`. Supported Makefile flags are `SDK`, `ARCHES` (armv7 default, armv4t-7s permitted), `CFLAGS`, `CPPFLAGS`. If you have cctools, use Makefile flag `LLVM-PREFIX=""`. To turn off PIC use `PIC=""`

## Status
About 35% of C89 is implemented.

Some of POSIX is implemented, e.g. sleep(), usleep(), select(), fork(), etc.

## License
All of my code licensed under MIT License

## Structure

1. ~./apple – Apple's code under APSL, C Start Up code~ (deprecated)
2. ./src – LibC's source
3. ./include – LibC's headers (stdint.h licensed under APSL)
4. ./src/sysindep – Probably system-independent code
5. ./src/sysdep (see below)
6. ./src/runtime/sysdep (C Startup, beta)
7. ./src/runtime/sysindep – stack canary
8. ./test – unittests (low priority, may be outdated)

## Porting
To port this one, you just need to make `./src/sysdep/*/*` files and replace `./include/sys/*` (and some other headers). Simplest way is to import symbols from gLibC
