#!/bin/bash

(cd ../.. ; ./make_all.sh > /dev/null 2> /dev/null; mv libc.a librt.a test/time)
cc -o test test.c -L. -lc -I../../include -lrt
./test
rm test libc.a librt.a
