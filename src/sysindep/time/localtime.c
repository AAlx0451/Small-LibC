#include <stdint.h>
#include <time.h>

#pragma clang diagnostic ignored "-Wreserved-identifier"
#include "tz_context.h"
#define SECS_PER_MIN 60
#define SECS_PER_HOUR 3600
#define SECS_PER_DAY 86400
#define EPOCH_YEAR 1970

static int __is_leap_loc(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

static const int __days_per_month_loc[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct tm *localtime(const time_t *timer) {
    static struct tm tm_result;
    tz_result_t tz_res;
    time_t local_time;
    long days, rem_secs;
    int year = EPOCH_YEAR;
    int month, wday;
    int success = 0;

    if(!timer)
        return NULL;
    if(!__libc_current_tz_db) {
        tzset();
    }
    if(__libc_current_tz_db) {
        success = tz_lookup(__libc_current_tz_db, (int64_t)*timer, &tz_res);
    }
    if(!success) {
        tz_res.gmtoff = 0;
        tz_res.is_dst = 0;
        tz_res.abbr = "UTC";
    }
    local_time = *timer + tz_res.gmtoff;
    days = local_time / SECS_PER_DAY;
    rem_secs = local_time % SECS_PER_DAY;
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
        int year_len = __is_leap_loc(year) ? 366 : 365;
        if(days >= 0) {
            if(days < year_len)
                break;
            days -= year_len;
            year++;
        } else {
            year--;
            year_len = __is_leap_loc(year) ? 366 : 365;
            days += year_len;
        }
    }
    tm_result.tm_year = year - 1900;
    tm_result.tm_yday = (int)days;

    for(month = 0; month < 12; month++) {
        int dim = __days_per_month_loc[month];
        if(month == 1 && __is_leap_loc(year))
            dim++;
        if(days < dim)
            break;
        days -= dim;
    }
    tm_result.tm_mon = month;
    tm_result.tm_mday = (int)days + 1;
    tm_result.tm_isdst = tz_res.is_dst;
    tm_result.tm_gmtoff = (long)tz_res.gmtoff;
    tm_result.tm_zone = (char *)(uintptr_t)tz_res.abbr; /* const char* -> char* cast */
    return &tm_result;
}
