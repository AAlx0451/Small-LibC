### Small-LibC
Just a devel lib designed for iOS easy to port. 

### List
* `mmap()`
* `calloc()`
* `exit()`
* `free()`
* `malloc()`
* `realloc()`
* `_exit()`
* `getpid()`
* `open()`
* `read()`
* `write()`
* `syscall()`
* `syscall0()`
* `syscall1()`
* `syscall2()`
* `syscall3()`
* `syscall4()`
* `syscall5()`
* `syscall6()`
* `fork()`

Also, [string.h](https://github.com/AAlx0451/string) git submodule included. 

For port purposes see `./include/*.h` and `./src/sysdep`. To build all (using iOS device with JB) run `./make_all.sh`. For cross compilation `cd ./src && make CROSS=1`

### Supported arches and OSes
iOS (6) fully supported. ARM BSD should work as well. For Linux you should replace syscall.c and fork.c. i386 syscall.h should work, but ARM assembly sysdeps wouldn't

For math.h recommended are OpenLibM and UNIXv7 LibM
