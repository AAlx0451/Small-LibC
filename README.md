## Welcome to Small-LibC
Small-LibC is an iOS development library, which implements some of standart C functions

## Get started
If you're on iOS, just run `./make_all.sh`, then you'll be able to use it as libc.a (recommended way)

If you're cross-compiling, go to ./src, then `make CROSS=1`. Supported Makefile flags are `SDK`, `ARCHES` (armv7 default, armv4t-7s permitted), `CFLAGS`, `CPPFLAGS`. If you have cctools, use Makefile flag `LLVM-PREFIX=""`. To turn off PIC use `PIC=""`

## Status
About 65% of C89 is implemented.

Some of POSIX is implemented, e.g. sleep(), usleep(), select(), fork(), etc.

## License
All of my code is a Public Domain

## Structure

1. ./src – LibC's source
2. ./include – LibC's headers (stdint.h licensed under APSL)
3. ./src/sysindep – Probably system-independent code
4. ./src/sysdep (see below)
5. ./src/runtime/sysdep (C Startup, beta)
5. ./src/runtime/sysindep – stack canary
6. ./src/math – libM
7. ./test – unittests (low priority, may be outdated)

