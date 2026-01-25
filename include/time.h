#ifndef TIME_H
#define TIME_H

#include <sys/types.h>
#include <stddef.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#define CLOCKS_PER_SEC 1000000
#define CLK_TCK 100

extern char *tzname[2];
extern long timezone;
extern int daylight;

struct tm {
        int     tm_sec;
        int     tm_min;
        int     tm_hour;
        int     tm_mday;
        int     tm_mon;
        int     tm_year;
        int     tm_wday;
        int     tm_yday;
        int     tm_isdst;
        long    tm_gmtoff;
        char    *tm_zone;
};

struct timespec {
	time_t tv_sec; 
	long   tv_nsec;
};

time_t time(time_t *tloc);
clock_t clock(void);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);
void tzset(void);
double difftime(time_t time1, time_t time0);
char *asctime(const struct tm *timeptr);
char *ctime(const time_t *timer);
time_t mktime(struct tm *tm);
size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *timeptr);

#endif /* TIME_H */
