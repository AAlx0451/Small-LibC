#ifndef UNISTD_H
#define UNISTD_H

#include <stddef.h> /* size_t */

/* TYPES */
typedef long ssize_t;

/* FUNCTIONS */
long syscall(long number, ...);
long syscall0(long number);
long syscall1(long number, long arg1);
long syscall2(long number, long arg1, long arg2);
long syscall3(long number, long arg1, long arg2, long arg3);
long syscall4(long number, long arg1, long arg2, long arg3, long arg4);
long syscall5(long number, long arg1, long arg2, long arg3, long arg4, long arg5);
long syscall6(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);
ssize_t write(int fd, const void *buf, size_t count);

#endif
