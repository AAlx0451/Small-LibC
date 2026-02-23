#include "tz_context.h"
#include <time.h>

#define SECS_PER_MIN 60
#define SECS_PER_HOUR 3600
#define SECS_PER_DAY 86400
#define EPOCH_YEAR 1970

static int __is_leap(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

static const int __days_per_month[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct tm *gmtime(const time_t *timer) {
    static struct tm tm_result;
    time_t time = *timer;
    long days, rem_secs;
    int year = EPOCH_YEAR;
    int month;
    int wday;

    if(!timer)
        return NULL;

    days = time / SECS_PER_DAY;
    rem_secs = time % SECS_PER_DAY;
    if(rem_secs < 0) {
        rem_secs += SECS_PER_DAY;
        days--;
    }

    tm_result.tm_hour = (int)(rem_secs / SECS_PER_HOUR);
    rem_secs %= SECS_PER_HOUR;
    tm_result.tm_min = (int)(rem_secs / SECS_PER_MIN);
    tm_result.tm_sec = (int)(rem_secs % SECS_PER_MIN);

    wday = (4 + days) % 7;
    if(wday < 0)
        wday += 7;
    tm_result.tm_wday = wday;

    while(1) {
        int year_len = __is_leap(year) ? 366 : 365;
        if(days >= 0) {
            if(days < year_len)
                break;
            days -= year_len;
            year++;
        } else {
            year--;
            year_len = __is_leap(year) ? 366 : 365;
            days += year_len;
        }
    }
    tm_result.tm_year = year - 1900;
    tm_result.tm_yday = (int)days;
    tm_result.tm_isdst = 0;

    for(month = 0; month < 12; month++) {
        int dim = __days_per_month[month];
        if(month == 1 && __is_leap(year))
            dim++;

        if(days < dim)
            break;
        days -= dim;
    }
    tm_result.tm_mon = month;
    tm_result.tm_mday = (int)days + 1;
    tm_result.tm_gmtoff = 0;
    tm_result.tm_zone = "UTC";

    return &tm_result;
}
