// unistd.h

#ifndef UNISTD_H
#define UNISTD_H

#ifndef NO_STDDEF
#include <stddef.h>
#else
typedef unsigned long size_t;
#endif

#include <sys/types.h>

typedef long ssize_t;

ssize_t write(int fd, const void *buf, size_t nbyte);
ssize_t read(int fd, void *buf, size_t nbyte);
__attribute__((noreturn)) void _exit(int status);

pid_t fork(void);
int execve(const char *pathname, char *const argv[], char *const envp[]);
pid_t waitpid(pid_t pid, int *wstatus, int options);

#endif // UNISTD_H
