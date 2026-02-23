#ifndef UNISTD_H
#define UNISTD_H

#include <features.h>

#if !defined(_ANSI)
#include <stddef.h> /* size_t */
#include <stdio.h> /* SEEK_SET, SEEK_END, SEEK_CUR */
#include <sys/types.h> /* NULL, types */

/* CONSTANTS */

#define _POSIX_VERSION 199009L
#define _POSIX_JOB_CONTROL 1
#define _POSIX_SAVED_IDS 1
#define _POSIX_CHOWN_RESTRICTED 1
#define _POSIX_NO_TRUNC 1
#define _POSIX_VDISABLE ((unsigned char)'\377')

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define F_OK 0
#define X_OK 1
#define W_OK 2
#define R_OK 4

#define _PC_LINK_MAX 1
#define _PC_MAX_CANON 2
#define _PC_MAX_INPUT 3
#define _PC_NAME_MAX 4
#define _PC_PATH_MAX 5
#define _PC_PIPE_BUF 6
#define _PC_CHOWN_RESTRICTED 7
#define _PC_NO_TRUNC 8
#define _PC_VDISABLE 9
#define _PC_NAME_CHARS_MAX 10
#define _PC_CASE_SENSITIVE 11
#define _PC_CASE_PRESERVING 12
#define _PC_EXTENDED_SECURITY_NP 13
#define _PC_AUTH_OPAQUE_NP 14
#define _PC_2_SYMLINKS 15
#define _PC_ALLOC_SIZE_MIN 16
#define _PC_ASYNC_IO 17
#define _PC_FILESIZEBITS 18
#define _PC_PRIO_IO 19
#define _PC_REC_INCR_XFER_SIZE 20
#define _PC_REC_MAX_XFER_SIZE 21
#define _PC_REC_MIN_XFER_SIZE 22
#define _PC_REC_XFER_ALIGN 23
#define _PC_SYMLINK_MAX 24
#define _PC_SYNC_IO 25
#define _PC_XATTR_SIZE_BITS 26

#define _SC_ARG_MAX 1
#define _SC_CHILD_MAX 2
#define _SC_CLK_TCK 3
#define _SC_NGROUPS_MAX 4
#define _SC_OPEN_MAX 5
#define _SC_JOB_CONTROL 6
#define _SC_SAVED_IDS 7
#define _SC_VERSION 8
#define _SC_BC_BASE_MAX 9
#define _SC_BC_DIM_MAX 10
#define _SC_BC_SCALE_MAX 11
#define _SC_BC_STRING_MAX 12
#define _SC_COLL_WEIGHTS_MAX 13
#define _SC_EXPR_NEST_MAX 14
#define _SC_LINE_MAX 15
#define _SC_RE_DUP_MAX 16
#define _SC_2_VERSION 17
#define _SC_2_C_BIND 18
#define _SC_2_C_DEV 19
#define _SC_2_CHAR_TERM 20
#define _SC_2_FORT_DEV 21
#define _SC_2_FORT_RUN 22
#define _SC_2_LOCALEDEF 23
#define _SC_2_SW_DEV 24
#define _SC_2_UPE 25
#define _SC_STREAM_MAX 26
#define _SC_TZNAME_MAX 27
#define _SC_ASYNCHRONOUS_IO 28
#define _SC_PAGESIZE 29

/* VARIABLES */
#if defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 2) || defined(_XOPEN_SOURCE)
extern char *optarg;
extern int optind, opterr, optopt;
#endif /* _DARWIN_C_SOURCE || _GNU_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 2) || _XOPEN_SOURCE */

extern char **environ;

/* FUNCTIONS */

#if defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) || defined(_XOPEN_SOURCE)
NORETURN void _exit(int status);
pid_t fork(void);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
int close(int fd);
int link(const char *oldpath, const char *newpath);
int unlink(const char *pathname);
int chdir(const char *path);
int chown(const char *path, uid_t owner, gid_t group);
pid_t getpid(void);
int setuid(uid_t uid);
uid_t getuid(void);
uid_t geteuid(void);
int access(const char *pathname, int mode);
pid_t getppid(void);
int dup(int oldfd);
int pipe(int pipefd[2]);
uid_t getegid(void);
gid_t getgid(void);
char *getlogin(void);
int execve(const char *pathname, char *const argv[], char *const envp[]);
int getgroups(int gidsetsize, gid_t grouplist[]);
pid_t getpgrp(void);
int setpgid(pid_t pid, pid_t pgid);
int dup2(int oldfd, int newfd);
int rmdir(const char *path);
pid_t setsid(void);
int setgid(gid_t gid);
long pathconf(const char *path, int name);
long fpathconf(int fd, int name);
off_t lseek(int fd, off_t offset, int whence);

int sleep(unsigned int seconds);
int pause(void);
int isatty(int fd);
int execv(const char *path, char *const argv[]);
int execl(const char *path, const char *arg, ...);
int execle(const char *path, const char *arg, ...);
int execvp(const char *file, char *const argv[]);
int execlp(const char *file, const char *arg, ...);
unsigned int alarm(unsigned int seconds);
pid_t tcgetpgrp(int fd);
int tcsetpgrp(int fd, pid_t pgrp);
long sysconf(int name);
char *getcwd(char *buf, size_t size);
char *ttyname(int fd);
#endif /* _DARWIN_C_SOURCE || _GNU_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 1) || _XOPEN_SOURCE */

#if defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 2) || defined(_XOPEN_SOURCE)
int getopt(int argc, char *const argv[], const char *optstring);
#endif /* _DARWIN_C_SOURCE || _GNU_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 2) || _XOPEN_SOURCE */

#if defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L) || defined(_XOPEN_SOURCE)
int fsync(int fd);
#endif /* _DARWIN_C_SOURCE || _GNU_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 199309L) || _XOPEN_SOURCE */

#if defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || defined(_XOPEN_SOURCE)
int fchown(int fd, uid_t owner, gid_t group);
int usleep(unsigned long);
int seteuid(uid_t euid);
int setpgrp(void);
int nice(int incr);
void sync(void);
int fchdir(int fd);
#endif /* _DARWIN_C_SOURCE || _GNU_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || _XOPEN_SOURCE */

#if defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || defined(_XOPEN_SOURCE)
int acct(const char *path);
int chroot(const char *path);
int brk(void *x);
void *sbrk(intptr_t size);
#endif /* _DARWIN_C_SOURCE || _GNU_SOURCE || _XOPEN_SOURCE */

#if defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE)

/* Internal */
#define _SYSCALL_GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define _SYSCALL_COUNT_ARGS(...) \
	    _SYSCALL_GET_NTH_ARG(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0)
#define _SYSCALL_CONCAT_IMPL(name, count) name##count
#define _SYSCALL_CONCAT(name, count) _SYSCALL_CONCAT_IMPL(name, count)

#define syscall(...) \
	    _SYSCALL_CONCAT(syscall, _SYSCALL_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

long syscall0(long number);
long syscall1(long number, long arg1);
long syscall2(long number, long arg1, long arg2);
long syscall3(long number, long arg1, long arg2, long arg3);
long syscall4(long number, long arg1, long arg2, long arg3, long arg4);
long syscall5(long number, long arg1, long arg2, long arg3, long arg4, long arg5);
long syscall6(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);
long syscall7(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7);
long syscall8(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7, long arg8);
#endif /* _DARWIN_C_SOURCE || _GNU_SOURCE */

#endif /* !_ANSI */

#endif /* !UNISTD_H */
