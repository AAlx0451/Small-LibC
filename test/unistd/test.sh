#!/bin/bash

(cd ../.. ; ./make_all.sh > /dev/null 2> /dev/null; mv libc.a test/unistd)
cc -o test unistd.c -L. -lc
./test
echo $?
rm test 1.txt libc.a
