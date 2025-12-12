## Welcome to Small-LibC
Small-LibC is an iOS development library, which implements some of C Stamdart Library functions

## Get started
If you're on iOS, just run `./make_all.sh`, then you'll be able to use it as libc.a (recommended way for on-device)

If you're cross-compiling, go to ./src, then `make CROSS=1`. Supported Makefile flags are `SDK`, `ARCHES` (armv7 default, 7s permitted, 2+ arches suppirted as `ARCHES=armv7,armv7s`), `CFLAGS`, `CPPFLAGS`. If you have cctools, use Makefile flag `LLVM-PREFIX=""` (default behavior is to call `$(LLVM-PREFIX)lipo`). To turn off PIC use `PIC=""` (default PIC is `-fPIC`)

## Status
About 65% of C89 is implemented.

Some of POSIX is implemented, e.g. sleep(), usleep(), select(), fork(), etc.

Full time and string are implemented. Full ANSI printf is implemented. StdLib is TODO, Locale is VERY low-priority

Note that LibM (IEEE&ANSI full BTW) and string are git submodules, so run git submodule update --recursive --init

## License
All of my code is a Public Domain (Unlicense). Note that 0ae2e51 commit and older are MIT-licensed

## Structure

1. ./src – LibC's source
2. ./include – LibC's headers (stdint.h licensed under APSL, be careful!)
3. ./src/sysindep – Probably system-independent code
4. ./src/sysdep – VERY system dependent code with inline asm
5. ./src/runtime/sysdep (C Startup fully compatible with Apple's, beta)
5. ./src/runtime/sysindep – stack canary, division runtime
6. ./src/math – libM (almost full, Public Domain)
7. ./test – unittests

## And one more thing....

### K&R status:

* currently, you can't pass 4.7 of 1st K&R (due to no getline (not an ANSI btw))

## Pssss.... MY REMORSE AND CONFESSION

* although it's tested and public domain, I MUST say it. it's mostly written using AI!!!! so I'm bad :)
