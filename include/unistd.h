#ifndef UNISTD_H
#define UNISTD_H

#include <stddef.h> /* size_t */

/* TYPES */
typedef long ssize_t;

/* FUNCTIONS */
long syscall(long number, ...);
ssize_t write(int fd, const void *buf, size_t count);

#endif
