#ifndef UNISTD_H
#define UNISTD_H

#include <stddef.h> /* size_t */
#include <sys/types.h> /* types, null ptr */

/* INTERNAL */
#if defined(__GNUC__) || defined(__clang__)
# define NORETURN __attribute__((noreturn))
#elif __STDC_VERSION__ >= 201112L
# define NORETURN _Noreturn
#else
# define NORETURN
#endif
#define _SYSCALL_GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, N, ...) N
#define _SYSCALL_COUNT_ARGS(...) _SYSCALL_GET_NTH_ARG(0, ##__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define _SYSCALL_CONCAT_IMPL(name, count) name##count
#define _SYSCALL_CONCAT(name, count) _SYSCALL_CONCAT_IMPL(name, count)

/* CONSTANTS */

// descriptors
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// should be defined at sys/types.h
#ifndef NULL
# define NULL ((void *)0)
#endif /* NULL */

/* FUNCTIONS */

// main syscall wrapper, up to 6 args
#define syscall(number, ...) \
    _SYSCALL_CONCAT(syscall, _SYSCALL_COUNT_ARGS(__VA_ARGS__))(number, ##__VA_ARGS__)

// safer functions
long syscall0(long number);
long syscall1(long number, long arg1);
long syscall2(long number, long arg1, long arg2);
long syscall3(long number, long arg1, long arg2, long arg3);
long syscall4(long number, long arg1, long arg2, long arg3, long arg4);
long syscall5(long number, long arg1, long arg2, long arg3, long arg4, long arg5);
long syscall6(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);

// syscall8 is special - it's made for apple mmap() and can't be replaced with up-to-six syscall
#ifdef __APPLE__
long syscall8(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7, long arg8);
#endif /* __APPLE */

// POSIX function based on syscalls
NORETURN void _exit(int status); // 1
pid_t fork(void); // 2
ssize_t read(int fd, void *buf, size_t count); // 3
ssize_t write(int fd, const void *buf, size_t count); // 4
int open(const char *pathname, int flags, ...); // 5
int close(int fd); // 6
pid_t getpid(void); // 20
int execve(const char *pathname, char *const argv[], char *const envp[]); // 59
off_t lseek(int fd, off_t offset, int whence); // 199

// POSIX functions - emulation
unsigned int sleep(unsigned int seconds); // SYS_select
unsigned int usleep(unsigned int microseconds); // SYS_select
void *brk(void *x); // SYS_mmap
void *sbrk(intptr_t size); // SYS_mmap


#endif
