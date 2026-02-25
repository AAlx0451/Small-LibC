#include <limits.h>
#include <time.h>
#pragma clang diagnostic ignored "-Wreserved-identifier"
#include "tz_context.h"

#define SECS_PER_MIN 60
#define SECS_PER_HOUR 3600
#define SECS_PER_DAY 86400
#define EPOCH_YEAR 1970

static int __is_leap(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

static const int __days_before_month[13] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

time_t mktime(struct tm *tm) {
    long long year, leaps_current, leaps_epoch;
    int mon, mday;
    long long total_days;
    time_t local_t, t;
    tz_result_t tz_res;
    struct tm *final_tm;

    mon = tm->tm_mon;
    year = tm->tm_year + 1900;

    if(mon < 0) {
        year += (mon / 12) - 1;
        mon = mon % 12 + 12;
    } else if(mon >= 12) {
        year += mon / 12;
        mon %= 12;
    }

    leaps_current = (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
    leaps_epoch = (EPOCH_YEAR - 1) / 4 - (EPOCH_YEAR - 1) / 100 + (EPOCH_YEAR - 1) / 400;

    total_days = (year - EPOCH_YEAR) * 365 + (leaps_current - leaps_epoch);

    total_days += __days_before_month[mon];
    if(mon > 1 && __is_leap((int)year)) {
        total_days++;
    }

    mday = tm->tm_mday;
    total_days += mday - 1;

    local_t = (time_t)(total_days * SECS_PER_DAY +
                       (long long)tm->tm_hour * SECS_PER_HOUR +
                       (long long)tm->tm_min * SECS_PER_MIN +
                       tm->tm_sec);

    if(!__libc_current_tz_db) {
        tzset();
    }

    tz_res.gmtoff = 0;
    if(__libc_current_tz_db) {
        tz_lookup(__libc_current_tz_db, local_t, &tz_res);
    }

    t = local_t - tz_res.gmtoff;

    if(__libc_current_tz_db) {
        tz_result_t tz_res2;
        tz_lookup(__libc_current_tz_db, t, &tz_res2);

        if(tz_res.gmtoff != tz_res2.gmtoff) {
            t = local_t - tz_res2.gmtoff;
        }
    }

    final_tm = localtime(&t);
    if(final_tm) {
        *tm = *final_tm;
    } else {
        return (time_t)-1;
    }

    return t;
}
