
#ifndef UTIME_H
#define UTIME_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE))
#include <sys/types.h>

struct utimbuf {
    time_t actime;
    time_t modtime;
};

int utime(const char *path, const struct utimbuf *times);

#endif /* !_ANSI && (_POSIX_C_SOURCE || _XOPEN_SOURCE || _GNU_SOURCE || _DARWIN_C_SOURCE) */

#endif /* !UTIME_H */
