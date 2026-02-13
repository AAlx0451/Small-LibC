#ifndef SYS_TYPES_H
#define SYS_TYPES_H

#ifndef NULL
# define NULL ((void *)0)
#endif

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#ifdef _XOPEN_SOURCE
# include <utime.h>
#endif

typedef int pid_t;
typedef unsigned short mode_t;
typedef int64_t off_t;
typedef long time_t;
typedef int32_t suseconds_t;
typedef unsigned int id_t;
typedef unsigned int uid_t;
typedef long ssize_t;
typedef unsigned long clock_t;
typedef uint32_t gid_t;
typedef int32_t dev_t;
typedef int32_t segsz_t;
typedef unsigned int u_int;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned long u_long;
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint32_t ino_t;
typedef uint16_t nlink_t;
typedef int64_t blkcnt_t;
typedef int32_t blksize_t;
typedef char* caddr_t;

#endif /* SYS_TYPES */
