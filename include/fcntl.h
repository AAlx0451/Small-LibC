#ifndef FCNTL_H
#define FCNTL_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))

#include <sys/stat.h>
#include <sys/types.h>

/* open() */
#define O_RDONLY 0x0000
#define O_WRONLY 0x0001
#define O_RDWR 0x0002
#define O_ACCMODE 0x0003
#define O_NONBLOCK 0x0004
#define O_APPEND 0x0008
#define O_CREAT 0x0200
#define O_TRUNC 0x0400
#define O_EXCL 0x0800
#define O_NOCTTY 0x20000
#define FD_CLOEXEC 1

#if _POSIX_C_SOURCE >= 200809L
# define O_DIRECTORY 0x100000
#endif /* _POSIX_C_SOURCE >= 200809L */

/* fcntl() */
#define F_DUPFD 0
#define F_GETFD 1
#define F_SETFD 2
#define F_GETFL 3
#define F_SETFL 4
#define F_GETLK 7
#define F_SETLK 8
#define F_SETLKW 9
#define F_RDLCK 1
#define F_WRLCK 3
#define F_UNLCK 2

/* structs */
struct flock {
  off_t l_start;
  off_t l_len;
  pid_t l_pid;
  short l_type;
  short l_whence;
};

/* functions */
int open(const char *pathname, int flags, ...);
int creat(const char *pathname, mode_t mode);
int fcntl(int fd, int cmd, ...);

#endif /* !ANSI && (_POSIX_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !FCNTL_H */
