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
	struct timeval it_interval;
	struct timeval it_value;
};

struct timezone {
	int tz_minuteswest;
	int tz_dsttime;
};

int gettimeofday(struct timeval *tv, void *tz);
int settimeofday(const struct timeval *tv, const struct timezone *tz);
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);

#endif
