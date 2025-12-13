#!/bin/bash

(cd ../.. ; ./make_all.sh > /dev/null 2> /dev/null; mv libc.a librt.a libm.a test/stdlib)
cc -o test test.c -L. -lc -I../../include -lrt
./test
rm -rf libc.a librt.a libm.a test
