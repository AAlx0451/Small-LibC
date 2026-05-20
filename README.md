## Welcome to Small-LibC!
Small-LibC is a small iOS 6 LibC implementation in Public Domain.

## Get started
If you're on iOS, just run `./make_all.sh`.
Also, this repo provides `mkfile` only for native compilation.

<details>
    <summary>If you're cross-compiling</summary>
If you're cross-compiling, go to ./src, then `make CROSS=1`. Supported Makefile flags are `SDK`, `ARCHES` (armv7 default, 4t,6,7s permitted, 2+ arches supported as `ARCHES=armv7,armv7s`)
</details>

## Status
* C90AMD1
* POSIX.1-1990
* POSIX.2-1992

## License
This project is a Public Domain and licensed under The Unlicense. Note that 570161d commit and older are MIT-licensed
 
## Structure

1. `./src` -- LibC's source
2. `./include` -- LibC's headers
3. `./src/c` -- Code written in C
4. `./src/asm` -- Code written in asm
5. `./src/runtime/asm` -- C Startup code and UBSan runtime
6. `./src/runtime/c` -- stack canary, arm compiler runtime routines
7. `./src/math` -- libM (mostly faithfully rounded)
8. `./test` -- Tests
9. `./doc` -- Documentation
