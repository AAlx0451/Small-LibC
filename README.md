## Small-LibC
This repo provides a WIP opensource LibC designed for iOS 6

## What's made
Right now, you can use:

* `exit()` and `_exit()` (the same functions temporarily, based on #1 syscall)
* `strlen()` (small implementarion in pure C)
* `write()` (syscall #4)
* `puts()` (pure C)
* `syscall()` (written in C inline assemly)
* `read()` (#3 syscall)
* `putchar()` (pure C)
* `memset()` (pure C)
* `printf()`, `vprintf()`, etc. (external code, source [here](https://github.com/mpaland/printf))
* stack protection

## How to use it
You should check ./example/makefile for more information

Briefly: `cc -o bin file.c -nostdlib -nostdinc -Iinclude -L. -lSystem -static`

## How to build it
Go to ./src, then `make`

## Known issues

1. Division

```
Undefined symbols for architecture armv7:
  "___divsi3", referenced from:
      _main in 1-bcf400.o
  "___modsi3", referenced from:
      _main in 1-bcf400.o
```

You should add `-Lgnu -lgcc` to your build command. By the way, you can re-build libC without printf.c (just mv it) and avoid this thing.

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

5. Deprecated subtype

```
ld: warning: CPU_SUBTYPE_ARM_ALL subtype is deprecated: gnu/libgcc.a
```

Sorry, but it just works. Beat it.
