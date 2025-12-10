#include <time.h>

static char asctime_buf[26];

static const char* wday_name[7] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static const char* mon_name[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static void format_2d(char *dest, int value) {
    dest[0] = (value / 10) + '0';
    dest[1] = (value % 10) + '0';
}

char *asctime(const struct tm *timeptr) {
    asctime_buf[0] = wday_name[timeptr->tm_wday][0];
    asctime_buf[1] = wday_name[timeptr->tm_wday][1];
    asctime_buf[2] = wday_name[timeptr->tm_wday][2];
    asctime_buf[3] = ' ';

    asctime_buf[4] = mon_name[timeptr->tm_mon][0];
    asctime_buf[5] = mon_name[timeptr->tm_mon][1];
    asctime_buf[6] = mon_name[timeptr->tm_mon][2];
    asctime_buf[7] = ' ';

    if (timeptr->tm_mday < 10) {
        asctime_buf[8] = ' ';
        asctime_buf[9] = timeptr->tm_mday + '0';
    } else {
        format_2d(&asctime_buf[8], timeptr->tm_mday);
    }
    asctime_buf[10] = ' ';

    format_2d(&asctime_buf[11], timeptr->tm_hour);
    asctime_buf[13] = ':';
    format_2d(&asctime_buf[14], timeptr->tm_min);
    asctime_buf[16] = ':';
    format_2d(&asctime_buf[17], timeptr->tm_sec);
    asctime_buf[19] = ' ';

    int year = timeptr->tm_year + 1900;
    asctime_buf[20] = (year / 1000) + '0';
    asctime_buf[21] = ((year / 100) % 10) + '0';
    asctime_buf[22] = ((year / 10) % 10) + '0';
    asctime_buf[23] = (year % 10) + '0';

    asctime_buf[24] = '\n';
    asctime_buf[25] = '\0';

    return asctime_buf;
}
