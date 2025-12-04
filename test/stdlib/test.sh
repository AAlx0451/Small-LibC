#!/bin/bash

(cd ../.. ; ./make_all.sh > /dev/null 2> /dev/null; mv libc.a test/stdlib)
cc -o test stdlib.c -L. -lc -I../../include
./test
rm test libc.a
