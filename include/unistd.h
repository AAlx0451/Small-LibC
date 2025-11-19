// unistd.h

#ifndef UNISTD_H
#define UNISTD_H

#ifndef NO_STDDEF
#include <stddef.h>
#else
typedef unsigned long size_t;
#endif

typedef long ssize_t;

ssize_t write(int fd, const void *buf, size_t nbyte);
ssize_t read(int fd, void *buf, size_t nbyte);
__attribute__((noreturn)) void _exit(int status);

#endif // UNISTD_H
