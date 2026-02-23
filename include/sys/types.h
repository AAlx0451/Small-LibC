#ifndef SYS_TYPES_H
#define SYS_TYPES_H

#include <features.h>
#include <stdint.h>
#include <stddef.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))

#ifndef NULL
# define NULL ((void *)0)
#endif /* !NULL */

typedef int pid_t;
typedef unsigned short mode_t;
typedef long time_t;
typedef long ssize_t;
typedef unsigned long clock_t;
typedef uint32_t gid_t;
typedef unsigned int uid_t;
typedef int32_t dev_t;
typedef uint32_t ino_t;
typedef uint16_t nlink_t;
typedef int64_t off_t;

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
typedef int32_t suseconds_t;
typedef unsigned int id_t;
typedef int64_t blkcnt_t;
typedef int32_t blksize_t;

# ifdef _XOPEN_SOURCE
#  include <utime.h>
# endif /* _XOPEN_SOURCE */
#endif /* _XOPEN_SOURCE || _GNU_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) */

#if defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE) || !defined(_POSIX_C_SOURCE)
typedef int32_t segsz_t;
typedef char* caddr_t;
typedef unsigned int u_int;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned long u_long;
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
#endif /* _GNU_SOURCE || _DARWIN_C_SOURCE || !_POSIX_C_SOURCE */

#endif /* !_ANSI && (_POSIX_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !SYS_TYPES_H */
