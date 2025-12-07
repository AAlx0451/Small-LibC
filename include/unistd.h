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
long syscall(long number, ...);

// safer functions
#ifndef NOSYSCALLN // please don't use for iOS, only for porting
long syscall0(long number);
long syscall1(long number, long arg1);
long syscall2(long number, long arg1, long arg2);
long syscall3(long number, long arg1, long arg2, long arg3);
long syscall4(long number, long arg1, long arg2, long arg3, long arg4);
long syscall5(long number, long arg1, long arg2, long arg3, long arg4, long arg5);
long syscall6(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);
#else
# define syscall0 syscall
# define syscall1 syscall
# define syscall2 syscall
# define syscall3 syscall
# define syscall4 syscall
# define syscall5 syscall
# define syscall6 syscall
#endif /* NOSYSCALLN */

// syscall8 is special - it's made for apple mmap() and can't be replaced with up-to-six syscall
#ifdef __APPLE__
long syscall8(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7, long arg8);
#endif /* __APPLE */

// POSIX function based on syscalls (numbers just for sort, see the actual syscall.h)
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
