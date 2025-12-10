#!/bin/bash

(cd ../.. ; ./make_all.sh > /dev/null 2> /dev/null; mv libc.a librt.a test/unistd)
cc -o test test.c -L. -lc -lrt
./test
rm test unistd_test_file.txt libc.a lseek_test_file.txt librt.a
