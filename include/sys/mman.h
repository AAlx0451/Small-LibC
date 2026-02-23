#ifndef SYS_MMAN_H
#define SYS_MMAN_H

#include <stddef.h>

#if !defined(_ANSI) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)
#include <sys/types.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t length);

#define PROT_NONE   0x00
#define PROT_READ   0x01
#define PROT_WRITE  0x02
#define PROT_EXEC   0x04

#define MAP_SHARED  0x0001
#define MAP_PRIVATE 0x0002
#define MAP_FIXED   0x0010
#define MAP_FAILED  ((void *)-1)

#if defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE)
#define MAP_ANON    0x1000
#define MAP_ANONYMOUS MAP_ANON
#endif /* _GNU_SOURCE || _DARWIN_C_SOURCE */

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500)
#define MCL_CURRENT 0x0001
#define MCL_FUTURE  0x0002
#endif /* (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 199309L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 500) */

#endif /* !_ANSI || _POSIX_C_SOURCE || _XOPEN_SOURCE || _GNU_SOURCE */

#endif /* !SYS_MMAN_H */
