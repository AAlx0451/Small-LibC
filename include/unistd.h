#ifndef UNISTD_H
#define UNISTD_H

#include <stddef.h> /* size_t */
#include <sys/types.h> /* types */

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
long syscall8(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7, long arg8);

ssize_t write(int fd, const void *buf, size_t count);
pid_t fork(void);
pid_t getpid(void);
__attribute__((noreturn)) void _exit(int status);
int close(int fd);
int open(const char *pathname, int flags, ...);
int execve(const char *pathname, char *const argv[], char *const envp[]);
off_t lseek(int fd, off_t offset, int whence);
unsigned int sleep(unsigned int seconds);
unsigned int usleep(unsigned int milliseconds);
void *brk(void *x);
void *sbrk(intptr_t size);

#endif
