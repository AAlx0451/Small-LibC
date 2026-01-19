#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

static int weeknumber(const struct tm *timeptr, int firstweekday);
static int iso8601wknum(const struct tm *timeptr);

static inline int append_char(char **s, char *end, const char c) {
    if (*s < end) {
        *(*s)++ = c;
        return 1;
    }
    return 0;
}

static inline int append_str(char **s, char *end, const char *str) {
    size_t len = strlen(str);
    if ((size_t)(end - *s) > len) {
        strcpy(*s, str);
        *s += len;
        return 1;
    }
    return 0;
}

static inline int format_num(char **s, char *end, long value, int width) {
    char buf[32];
    char *p = buf + sizeof(buf) - 1;
    int is_neg = value < 0;

    *p = '\0';

    if (is_neg) {
        value = -value;
    }
    
    do {
        *--p = (char)('0' + (value % 10));
        value /= 10;
        width--;
    } while (value > 0);

    while (width-- > 0) {
        *--p = '0';
    }

    if (is_neg) {
        *--p = '-';
    }

    return append_str(s, end, p);
}


static inline int min(int a, int b) { return (a < b ? a : b); }
static inline int max(int a, int b) { return (a > b ? a : b); }
#define range(low, item, hi) max(low, min(item, hi))

static const char *days_a(int index) {
    static const char *data[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    return data[index];
}

static const char *days_l(int index) {
    static const char *data[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
    return data[index];
}

static const char *months_a(int index) {
#ifdef HAVE_NL_LANGINFO
    static const nl_item data[] = { ABMON_1, ABMON_2, ABMON_3, ABMON_4, ABMON_5, ABMON_6, ABMON_7, ABMON_8, ABMON_9, ABMON_10, ABMON_11, ABMON_12 };
    return nl_langinfo(data[index]);
#else
    static const char *data[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    return data[index];
#endif
}

static const char *months_l(int index) {
#ifdef HAVE_NL_LANGINFO
    static const nl_item data[] = { MON_1, MON_2, MON_3, MON_4, MON_5, MON_6, MON_7, MON_8, MON_9, MON_10, MON_11, MON_12 };
    return nl_langinfo(data[index]);
#else
    static const char *data[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
    return data[index];
#endif
}

static const char *ampm(int index) {
#ifdef HAVE_NL_LANGINFO
    static const nl_item data[] = { AM_STR, PM_STR };
    return nl_langinfo(data[index]);
#else
    static const char *data[] = { "AM", "PM" };
    return data[index];
#endif
}

size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *timeptr) {
    char *start = s;
    char *endp = s + maxsize;
    static short first = 1;

#ifdef POSIX_SEMANTICS
    static char *savetz = NULL;
    char *tz;
#endif

    if (s == NULL || format == NULL || timeptr == NULL || maxsize == 0)
        return 0;

    if (strchr(format, '%') == NULL) {
        if (strlen(format) + 1 > maxsize) return 0;
        strcpy(s, format);
        return strlen(s);
    }

#ifndef POSIX_SEMANTICS
    if (first) {
        tzset();
        first = 0;
    }
#else
    tz = getenv("TZ");
    if (first || (tz && (!savetz || strcmp(tz, savetz) != 0))) {
        tzset();
        first = 0;
        free(savetz);
        savetz = tz ? strdup(tz) : NULL;
    }
#endif

    for (; *format && (s < endp - 1); format++) {
        if (*format != '%') {
            *s++ = *format;
            continue;
        }

    again:
        switch (*++format) {
            case '\0':
                *s++ = '%';
                goto out;

            case '%':
                if (!append_char(&s, endp, '%')) return 0;
                continue;

            case 'a':
                if (!append_str(&s, endp, days_a(range(0, timeptr->tm_wday, 6)))) return 0;
                break;

            case 'A':
                if (!append_str(&s, endp, days_l(range(0, timeptr->tm_wday, 6)))) return 0;
                break;

            case 'b':
            case 'h':
                if (!append_str(&s, endp, months_a(range(0, timeptr->tm_mon, 11)))) return 0;
                break;

            case 'B':
                if (!append_str(&s, endp, months_l(range(0, timeptr->tm_mon, 11)))) return 0;
                break;

            case 'c': {
#ifdef HAVE_NL_LANGINFO
                char c_fmt[128];
                strftime(c_fmt, sizeof(c_fmt), nl_langinfo(D_T_FMT), timeptr);
                if (!append_str(&s, endp, c_fmt)) return 0;
#else
                if (!append_str(&s, endp, days_a(range(0, timeptr->tm_wday, 6)))) return 0;
                if (!append_char(&s, endp, ' ')) return 0;
                if (!append_str(&s, endp, months_a(range(0, timeptr->tm_mon, 11)))) return 0;
                if (!append_char(&s, endp, ' ')) return 0;
                if (timeptr->tm_mday < 10) if (!append_char(&s, endp, ' ')) return 0;
                if (!format_num(&s, endp, timeptr->tm_mday, 1)) return 0;
                if (!append_char(&s, endp, ' ')) return 0;
                if (!format_num(&s, endp, range(0, timeptr->tm_hour, 23), 2)) return 0;
                if (!append_char(&s, endp, ':')) return 0;
                if (!format_num(&s, endp, range(0, timeptr->tm_min, 59), 2)) return 0;
                if (!append_char(&s, endp, ':')) return 0;
                if (!format_num(&s, endp, range(0, timeptr->tm_sec, 60), 2)) return 0;
                if (!append_char(&s, endp, ' ')) return 0;
                if (!format_num(&s, endp, timeptr->tm_year + 1900L, 4)) return 0;
#endif
                break;
            }

            case 'C':
                if (!format_num(&s, endp, (timeptr->tm_year + 1900L) / 100, 2)) return 0;
                break;

            case 'd':
                if (!format_num(&s, endp, range(1, timeptr->tm_mday, 31), 2)) return 0;
                break;

            case 'D':
                if (!format_num(&s, endp, range(0, timeptr->tm_mon, 11) + 1, 2)) return 0;
                if (!append_char(&s, endp, '/')) return 0;
                if (!format_num(&s, endp, range(1, timeptr->tm_mday, 31), 2)) return 0;
                if (!append_char(&s, endp, '/')) return 0;
                if (!format_num(&s, endp, timeptr->tm_year % 100, 2)) return 0;
                break;
            
            case 'e':
                if (timeptr->tm_mday < 10) if (!append_char(&s, endp, ' ')) return 0;
                if (!format_num(&s, endp, range(1, timeptr->tm_mday, 31), 1)) return 0;
                break;
            
            case 'E':
            case 'O':
                goto again;

            case 'F':
                if (!format_num(&s, endp, timeptr->tm_year + 1900L, 4)) return 0;
                if (!append_char(&s, endp, '-')) return 0;
                if (!format_num(&s, endp, range(0, timeptr->tm_mon, 11) + 1, 2)) return 0;
                if (!append_char(&s, endp, '-')) return 0;
                if (!format_num(&s, endp, range(1, timeptr->tm_mday, 31), 2)) return 0;
                break;

            case 'g':
            case 'G': {
                int w = iso8601wknum(timeptr);
                long y = 1900L + timeptr->tm_year;
                if (timeptr->tm_mon == 11 && w == 1) y++;
                else if (timeptr->tm_mon == 0 && w >= 52) y--;
                
                if (*format == 'G') {
                    if (!format_num(&s, endp, y, 4)) return 0;
                } else {
                    if (!format_num(&s, endp, y % 100, 2)) return 0;
                }
                break;
            }
            
            case 'H':
                if (!format_num(&s, endp, range(0, timeptr->tm_hour, 23), 2)) return 0;
                break;

            case 'I': {
                int i = range(0, timeptr->tm_hour, 23);
                if (i == 0) i = 12;
                else if (i > 12) i -= 12;
                if (!format_num(&s, endp, i, 2)) return 0;
                break;
            }

            case 'j':
                if (!format_num(&s, endp, timeptr->tm_yday + 1, 3)) return 0;
                break;

            case 'm':
                if (!format_num(&s, endp, range(0, timeptr->tm_mon, 11) + 1, 2)) return 0;
                break;
            
            case 'M':
                if (!format_num(&s, endp, range(0, timeptr->tm_min, 59), 2)) return 0;
                break;

            case 'n':
                if (!append_char(&s, endp, '\n')) return 0;
                break;

            case 'p':
                if (!append_str(&s, endp, ampm(timeptr->tm_hour < 12 ? 0 : 1))) return 0;
                break;

            case 'r': {
                int i = range(0, timeptr->tm_hour, 23);
                if (i == 0) i = 12;
                else if (i > 12) i -= 12;
                if (!format_num(&s, endp, i, 2)) return 0;
                if (!append_char(&s, endp, ':')) return 0;
                if (!format_num(&s, endp, range(0, timeptr->tm_min, 59), 2)) return 0;
                if (!append_char(&s, endp, ':')) return 0;
                if (!format_num(&s, endp, range(0, timeptr->tm_sec, 60), 2)) return 0;
                if (!append_char(&s, endp, ' ')) return 0;
                if (!append_str(&s, endp, ampm(timeptr->tm_hour < 12 ? 0 : 1))) return 0;
                break;
            }

            case 'R':
                if (!format_num(&s, endp, range(0, timeptr->tm_hour, 23), 2)) return 0;
                if (!append_char(&s, endp, ':')) return 0;
                if (!format_num(&s, endp, range(0, timeptr->tm_min, 59), 2)) return 0;
                break;

            case 's': {
                struct tm non_const_timeptr = *timeptr;
                if (!format_num(&s, endp, mktime(&non_const_timeptr), 0)) return 0;
                break;
            }

            case 'S':
                if (!format_num(&s, endp, range(0, timeptr->tm_sec, 60), 2)) return 0;
                break;

            case 't':
                if (!append_char(&s, endp, '\t')) return 0;
                break;

            case 'T':
            the_time:
                if (!format_num(&s, endp, range(0, timeptr->tm_hour, 23), 2)) return 0;
                if (!append_char(&s, endp, ':')) return 0;
                if (!format_num(&s, endp, range(0, timeptr->tm_min, 59), 2)) return 0;
                if (!append_char(&s, endp, ':')) return 0;
                if (!format_num(&s, endp, range(0, timeptr->tm_sec, 60), 2)) return 0;
                break;
            
            case 'u':
                if (!format_num(&s, endp, timeptr->tm_wday == 0 ? 7 : timeptr->tm_wday, 1)) return 0;
                break;
            
            case 'U':
                if (!format_num(&s, endp, weeknumber(timeptr, 0), 2)) return 0;
                break;

            case 'V':
                if (!format_num(&s, endp, iso8601wknum(timeptr), 2)) return 0;
                break;

            case 'w':
                if (!format_num(&s, endp, range(0, timeptr->tm_wday, 6), 1)) return 0;
                break;

            case 'W':
                if (!format_num(&s, endp, weeknumber(timeptr, 1), 2)) return 0;
                break;
            
            case 'x': {
#ifdef HAVE_NL_LANGINFO
                char x_fmt[128];
                strftime(x_fmt, sizeof(x_fmt), nl_langinfo(D_FMT), timeptr);
                if (!append_str(&s, endp, x_fmt)) return 0;
#else
                if (!format_num(&s, endp, range(0, timeptr->tm_mon, 11) + 1, 2)) return 0;
                if (!append_char(&s, endp, '/')) return 0;
                if (!format_num(&s, endp, range(1, timeptr->tm_mday, 31), 2)) return 0;
                if (!append_char(&s, endp, '/')) return 0;
                if (!format_num(&s, endp, timeptr->tm_year % 100, 2)) return 0;
#endif
                break;
            }

            case 'X':
#ifdef HAVE_NL_LANGINFO
                {
                    char X_fmt[128];
                    strftime(X_fmt, sizeof(X_fmt), nl_langinfo(T_FMT), timeptr);
                    if (!append_str(&s, endp, X_fmt)) return 0;
                }
#else
                goto the_time;
#endif
                break;

            case 'y':
                if (!format_num(&s, endp, timeptr->tm_year % 100, 2)) return 0;
                break;

            case 'Y':
                if (!format_num(&s, endp, 1900L + timeptr->tm_year, 4)) return 0;
                break;

            case 'z': {
                long off = 0;
                if (timeptr->tm_isdst < 0) break;
                off = timeptr->tm_gmtoff / 60;
                if (off < 0) {
                    if (!append_char(&s, endp, '-')) return 0;
                    off = -off;
                } else {
                    if (!append_char(&s, endp, '+')) return 0;
                }
                if (!format_num(&s, endp, off / 60, 2)) return 0;
                if (!format_num(&s, endp, off % 60, 2)) return 0;
                break;
            }

            case 'Z':
                if (timeptr->tm_zone && !append_str(&s, endp, timeptr->tm_zone)) return 0;
                break;
            
            default: {
                char dbuf[3] = {'%', *format, '\0'};
                if (!append_str(&s, endp, dbuf)) return 0;
                break;
            }
        }
    }

out:
    if (s < endp && *format == '\0') {
        *s = '\0';
        return (s - start);
    }
    
    return 0;
}

static int isleap(long year) {
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

static int iso8601wknum(const struct tm *timeptr) {
    int weeknum;
    int jan1day;
    long year = 1900L + timeptr->tm_year;

    weeknum = weeknumber(timeptr, 1);
    
    jan1day = timeptr->tm_wday - (timeptr->tm_yday % 7);
    if (jan1day < 0) jan1day += 7;

    switch (jan1day) {
        case 1: break;
        case 2: case 3: case 4:
            weeknum++;
            break;
        case 5: case 6: case 0:
            if (weeknum == 0) {
                struct tm dec31ly = *timeptr;
                dec31ly.tm_year--;
                dec31ly.tm_mon = 11;
                dec31ly.tm_mday = 31;
                dec31ly.tm_wday = (jan1day == 0) ? 6 : jan1day - 1;
                dec31ly.tm_yday = 364 + isleap(dec31ly.tm_year + 1900L);
                weeknum = iso8601wknum(&dec31ly);
            }
            break;
    }

    if (timeptr->tm_mon == 11) {
        int wday = timeptr->tm_wday;
        int mday = timeptr->tm_mday;
        if ((wday == 1 && (mday >= 29 && mday <= 31)) ||
            (wday == 2 && (mday >= 30 || mday == 31)) ||
            (wday == 3 && mday == 31)) {
            weeknum = 1;
        }
    }
    return weeknum;
}

static int weeknumber(const struct tm *timeptr, int firstweekday) {
    int wday = timeptr->tm_wday;
    int ret;
    if (firstweekday == 1) {
        wday = (wday == 0) ? 6 : wday - 1;
    }
    ret = ((timeptr->tm_yday + 7 - wday) / 7);
    return (ret < 0) ? 0 : ret;
}
