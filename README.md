## Welcome to Small-LibC!
Small-LibC is a small iOS 6 LibC implementation in Public Domain

## Get started
If you're on iOS, just run `./make_all.sh`.

<details>
    <summary>If you're cross-compiling</summary>
If you're cross-compiling, go to ./src, then `make CROSS=1`. Supported Makefile flags are `SDK`, `ARCHES` (armv7 default, 4t,6,7s permitted, 2+ arches supported as `ARCHES=armv7,armv7s`)
</details>

## Status
* Almost full ANSI C89
* Almost full ISO C90AMD1
* Full POSIX.1-1990
* Full POSIX.2-1992

Note math is not currently implemented

## Future
Full math, then C99, then tests, then POSIX.1c-1995, POSIX.1-2001, C11, XPG, etc.

## License
This project is a Public Domain and licensed under The Unlicense. Note that 570161d commit and older are MIT-licensed
 
## Structure

1. `./src` – LibC's source
2. `./include` – LibC's headers
3. `./src/c` – Code written in C
4. `./src/asm` – Code written in asm
5. `./src/runtime/asm` – C Startup code and UBSan runtime
6. `./src/runtime/c` – stack canary, division runtime
7. `./src/math` – libM (TODO)
8. `./test` - Tests (3-5% coverage)
