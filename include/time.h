#ifndef TIME_H
#define TIME_H

#include <sys/types.h>

#ifndef NULL
#define NULL ((void*)0)
#endif
#define CLOCKS_PER_SEC 1000000


struct tm {
        int     tm_sec;         /* seconds after the minute [0-60] */
        int     tm_min;         /* minutes after the hour [0-59] */
        int     tm_hour;        /* hours since midnight [0-23] */
        int     tm_mday;        /* day of the month [1-31] */
        int     tm_mon;         /* months since January [0-11] */
        int     tm_year;        /* years since 1900 */
        int     tm_wday;        /* days since Sunday [0-6] */
        int     tm_yday;        /* days since January 1 [0-365] */
        int     tm_isdst;       /* Daylight Savings Time flag */
        long    tm_gmtoff;      /* offset from CUT in seconds */
        char    *tm_zone;       /* timezone abbreviation */
};

time_t time(time_t *tloc);
clock_t clock(void);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);
void tzset(void);

#endif /* TIME_H */
