#ifndef TIME_H
#define TIME_H

#include <stddef.h>
#include <features.h>

#ifndef NULL
#define NULL ((void*)0)
#endif /* !NULL */

#define CLOCKS_PER_SEC 1000000

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) || defined(_XOPEN_SOURCE))
#include <sys/types.h>
#define CLK_TCK 100
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _GNU_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 1) || _XOPEN_SOURCE) */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) || defined(_XOPEN_SOURCE))
extern char *tzname[2];
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _GNU_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 1) || _XOPEN_SOURCE) */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500))
extern long timezone;
extern int daylight;
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _GNU_SOURCE || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 500)) */

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
#if defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE)
    long tm_gmtoff;
    char *tm_zone;
#else
    char ___padding[sizeof(long) + sizeof(char*)];
#endif /* _DARWIN_C_SOURCE || _GNU_SOURCE */
};

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) || \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600))
struct timespec {
    time_t tv_sec;
    long tv_nsec;
};
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _GNU_SOURCE || (__STDC_VERSION__ && __STDC_VERSION__ >= 201112L) || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600)) */

time_t time(time_t *tloc);
clock_t clock(void);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);
double difftime(time_t time1, time_t time0);
char *asctime(const struct tm *timeptr);
char *ctime(const time_t *timer);
time_t mktime(struct tm *tm);
size_t strftime(char *restrict s, size_t maxsize, const char *restrict format, const struct tm *restrict timeptr);

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
void tzset(void);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _GNU_SOURCE || _POSIX_C_SOURCE || _XOPEN_SOURCE) */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE) || defined(_XOPEN_SOURCE))
int stime(const time_t *t);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _GNU_SOURCE || _XOPEN_SOURCE) */

#endif /* !TIME_H */
