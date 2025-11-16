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

You should add `-Lgcc -lgcc` to your build command

2. Stack protector

```
Undefined symbols for architecture armv7:
  "___stack_chk_fail", referenced from:
      _main in 1-1e6844.o
  "___stack_chk_guard", referenced from:
      _main in 1-1e6844.o
```

Add `-fno-stack-protector`

3. Dynamic lib (1)

```
ld: dynamic main executables must link with libSystem.dylib for architecture armv7
```

Add `-static`

4. Dynamic lib (2)

```
ld: symbol dyld_stub_binder not found (normally in libSystem.dylib).  Needed to perform lazy binding to function _exit for architecture armv7
```

Do NOT use this lib as dynamic. But you can link Small-LibC with libSystem and it should work
