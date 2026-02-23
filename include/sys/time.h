#ifndef SYS_TIME_H
#define SYS_TIME_H

#include <features.h>

#if !defined(_ANSI) && (defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || defined(_DARWIN_C_SOURCE))

#include <sys/types.h>

# define ITIMER_REAL    0
# define ITIMER_VIRTUAL 1
# define ITIMER_PROF    2

struct timeval
{
    time_t      tv_sec;
    suseconds_t tv_usec;
};

struct itimerval {
    struct timeval it_interval;
    struct timeval it_value;
};

int gettimeofday(struct timeval *restrict tv, void *restrict tz);
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
int utimes(const char *path, const struct timeval times[2]);

#if defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE)

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

int settimeofday(const struct timeval *tv, const struct timezone *tz);

#endif /* _GNU_SOURCE || _DARWIN_C_SOURCE */

#endif /* !_ANSI && (_XOPEN_SOURCE || _GNU_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || _DARWIN_C_SOURCE */

#endif /* !SYS_TIME_H */
