// unistd.h

#ifndef UNISTD_H
#define UNISTD_H

#include <stddef.h>

typedef long ssize_t;

ssize_t write(int fd, const void *buf, size_t nbyte);
ssize_t read(int fd, void *buf, size_t nbyte);
__attribute__((noreturn)) void _exit(int status);

#endif // UNISTD_H
