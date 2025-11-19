## Small-LibC
This repo provides a WIP opensource LibC designed for iOS 6

## What's made
Right now, you can use:

* `string.h` full implementation (Pure C, I used AI for because of it's simplicity and system-independence (header written myself). btw maybe I'll release it separately)
* `write()` (syscall #4)
* `puts()` (pure C)
* `syscall()` (written in C inline assemly)
* `read()` (#3 syscall)
* `putchar()` (pure C)
* `printf()`, `vprintf()`, etc. (external code, source [here](https://github.com/mpaland/printf))
* `malloc()`, `free()`, etc. (very changed external code, see below, but its buggy)
* `mmap()`, `munmap()` (syscalls)
* `putchar()` (and `_putchar()` as internal for `printf()` – pure C)
* stack protection
* `ctype.h` (external code, source [here](https://github.com/DeidaraC/ctype.h))
* `stdlib.h` (excluding malloc – pure C with some AI help (NOT TESTED!!!))

## How to use it
You should check ./example/makefile for more information

Briefly: `cc -o bin file.c -nostdlib -nostdinc -Iinclude -L. -lSystem -static`

## How to build it
Go to ./src, then `make`. For (cross-compile: `make -j16 CROSS=1 ARCHES=armv7,armv7s`. Notice you'll need an SDK)

## Known issues

1. Division

```
Undefined symbols for architecture armv7:
  "___divsi3", referenced from:
      _main in 1-bcf400.o
  "___modsi3", referenced from:
      _main in 1-bcf400.o
```

You should add `-Lllvm -lrt` to your build command.

2. Dynamic lib (1)

```
ld: dynamic main executables must link with libSystem.dylib for architecture armv7
```

Add `-static`

3. Dynamic lib (2)

```
ld: symbol dyld_stub_binder not found (normally in libSystem.dylib).  Needed to perform lazy binding to function _exit for architecture armv7
```

Do NOT use this lib as dynamic. But you can link Small-LibC with libSystem and it should work

4. stdarg.h

```
include/stdarg.h:4:15: fatal error: 'stdarg.h' file not found
#include_next <stdarg.h>
              ^~~~~~~~~~
```

You should add `-I/usr/share/llvm/lib/clang/6.0.1/include` if you use my clang (see ./example/makefile). Also you can try add `-DNO_STDARG`, if you don’t need stdarg for you app (but for building stdarg a is hard dependency)

```
~/Small-LibC $ clang -o hello helloworld.c start.o -nostdlib -nostdinc -Iinclude -L. -lSystem -static -Lgnu -lgcc                                                               In file included from helloworld.c:1:
In file included from include/stdio.h:6:
include/stdarg.h:4:15: fatal error: 'stdarg.h' file not found
#include_next <stdarg.h>
              ^~~~~~~~~~
1 error generated.
~/Small-LibC $ clang -o hello helloworld.c start.o -nostdlib -nostdinc -Iinclude -L. -lSystem -static -Lgnu -lgcc -DNO_STDARG
~/Small-LibC $ ./hello
hello world
```

5. Strange source at ./ext

```
/private/var/lina/Small-LibC $ ls ext/csu
crt.c  dyld_glue.s  etc.tar.xz  start.s
/private/var/lina/Small-LibC $
```

To reduce source files count, I compressed them. Most important files are decompressed. Use tar

## C89 Status
~50% of C89 is implemented.

* string.h – 100%
* stdlib.h – ~100%
* stdio.h – ~10%
* ctype.h – 100%
* math.h – NaN (OpenLibM)
* time.h – 0%
* `assert.h`, `locale.h`, `setjmp.h`, `signal.h` – 0%
* `errno.h` – 100%
* `limits.h` – NaN/0% (Clang provides this one, but "System headers include a number of constants from POSIX in <limits.h>.")
* `float.h` – NaN/0% (Clang need a system implementation for it's own header)
* `stdarg.h` – NaN (Clang fully provides)
* `stddef.h` – NaN (Clang fully provides, but it looks like it can include system one)

## External code
1. Apple's [Csu 85](https://github.com/apple-oss-distributions/Csu), [tarball](https://github.com/apple-oss-distributions/Csu/archive/refs/tags/Csu-85.tar.gz) – licensed under APSL, see ./apple/LICENSE
2. LLVM's [compiler-rt](https://github.com/llvm/llvm-project), [tarball](https://releases.llvm.org/6.0.1/compiler-rt-6.0.1.src.tar.xz) – licensed under Apache 2.0 with LLVM exceptions, see ./llvm/LICENSE
3. mpaland's [printf](https://github.com/mpaland/printf) – licensed under MIT License, see ./ext/printf/LICENSE
4. RAGUL1902's [malloc](https://github.com/RAGUL1902/Dynamic-Memory-Allocation-in-C) with various changes – licensed under MIT Licence
5. JuliaMath's [OpenLibM](https://github.com/JuliaMath/openlibm) – partly licensed under BSD, ISC, MIT and Sun License (Tests under GPL, but will be removed soon); not integrated, xz-ed
6. DeidaraC's [ctype.h](https://github.com/DeidaraC/ctype.h) – licensed under MIT License
