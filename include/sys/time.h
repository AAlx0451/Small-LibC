#ifndef SYS_TIME_H
# define SYS_TIME_H

# include <sys/types.h>
# include <unistd.h>

# if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || (_POSIX_C_SOURCE >= 200112L)

struct timeval
{
    time_t      tv_sec;
    suseconds_t tv_usec;
};

int gettimeofday(struct timeval *restrict tv, void *restrict tz);

# endif

# if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)

#  define ITIMER_REAL    0
#  define ITIMER_VIRTUAL 1
#  define ITIMER_PROF    2

struct itimerval {
    struct timeval it_interval;
    struct timeval it_value;
};

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

int settimeofday(const struct timeval *tv, const struct timezone *tz);
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
int utimes(const char *path, const struct timeval times[2]);

# endif

#endif
