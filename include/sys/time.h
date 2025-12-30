#ifndef SYS_TIME_H
# define SYS_TIME_H

# include <sys/types.h>

# define ITIMER_REAL    0
# define ITIMER_VIRTUAL 1
# define ITIMER_PROF    2

struct timeval
{
        time_t tv_sec;
        suseconds_t    tv_usec;
};

struct itimerval {
    struct timeval it_interval; /* Timer interval (0 for single-shot) */
    struct timeval it_value;    /* Current value (0 to disable) */
};

int gettimeofday(struct timeval *tv, void *tz);
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);

#endif
