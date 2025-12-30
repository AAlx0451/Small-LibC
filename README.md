## Welcome to Small-LibC!
Small-LibC is an iOS development library, which implements full ANSI C89, some C99 and POSIX functions

## Get started
If you're on iOS, just run `./make_all.sh`, then you'll be able to use it as libc.a (recommended way for on-device)

<details>
    <summary>If you're cross-compiling</summary>
If you're cross-compiling, go to ./src, then `make CROSS=1`. Supported Makefile flags are `SDK`, `ARCHES` (armv7 default, 7s permitted, 2+ arches suppirted as `ARCHES=armv7,armv7s`), `CFLAGS`, `CPPFLAGS`. If you have cctools, use Makefile flag `LLVM-PREFIX=""` (default behavior is to call `$(LLVM-PREFIX)lipo`). To turn off PIC use `PIC=""` (default PIC is `-fPIC`)
</details>

## Status
* FULL ANSI C89
* POSIX isn't targeted, but some useful functions are implemented, including obscure like dprintf()
* Some Mach syscalls implemented with libMach. To compile it (required for stdio), use `./getlibmach.sh`. It's very small.
* ANSI requires support for "C" locale only, which this library does. You can find runelocale parser at `./src/sysindep/locale/unused`

Note that LibM and string are git submodules, so run git submodule update --recursive --init before compiling (of course, if you need it. if don't, just `touch src/math/dummy.c` and it will pass)

## License
This project is a Public Domain and licensed under The Unlicense. Note that 570161d commit and older are MIT-licensed
 
## Structure

1. ./src – LibC's source
2. ./include – LibC's headers (some `sys` licensed under APSL, be careful!)
3. ./src/sysindep – Probably system-independent code
4. ./src/sysdep – "system-dependent" code: uses inline asm. can be used like `xnu-tree/libsyscall/custom`
5. ./src/runtime/sysdep – C Startup code
6. ./src/runtime/sysindep – stack canary, division runtime
7. ./src/math – libM

## Notes

### Why sysindep us "probably" system-independent?

TL;DL: I'm lazy

Actually, it should be named "c", not "sysindep", but I don't want to change it – I'm too lazy to rewrite Makefile and change `string` submodude path

### Is it compatible?

TL;DR: no, it's not.

The only thing that must be compatible is runtime and it is. Actually, this library can be used with libSystem if you'll remove `./src/sysindep/stdio` – it's uncompatible.

### Is dynamic linking supported?

TL;DR: no

See previous pls. And of course it can't replace libSystem

### Is it thread-safe? What about pthreads?

TL;DR: malloc and stdio are safe. libmach is required for pthreads

`*alloc`, `free`, stdio functions are protected with spinlocks.
