#!/bin/bash

(cd ../.. ; ./make_all.sh > /dev/null 2> /dev/null; mv libc.a test/unistd)
cc -o test unistd.c -L. -lc
./test
rm test unistd_test_file.txt libc.a lseek_test_file.txt
