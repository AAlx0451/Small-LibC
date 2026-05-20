#include <langinfo.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

static const nl_item abday_i[] = {ABDAY_1, ABDAY_2, ABDAY_3, ABDAY_4, ABDAY_5, ABDAY_6, ABDAY_7};
static const nl_item day_i[] = {DAY_1, DAY_2, DAY_3, DAY_4, DAY_5, DAY_6, DAY_7};
static const nl_item abmon_i[] = {ABMON_1,
                                  ABMON_2,
                                  ABMON_3,
                                  ABMON_4,
                                  ABMON_5,
                                  ABMON_6,
                                  ABMON_7,
                                  ABMON_8,
                                  ABMON_9,
                                  ABMON_10,
                                  ABMON_11,
                                  ABMON_12};
static const nl_item mon_i[] =
    {MON_1, MON_2, MON_3, MON_4, MON_5, MON_6, MON_7, MON_8, MON_9, MON_10, MON_11, MON_12};
static const nl_item ampm_i[] = {AM_STR, PM_STR};

#define RANGE(low, item, hi) ((item) < (low) ? (low) : ((item) > (hi) ? (hi) : (item)))

#define OUTSTR(str)                                                                                \
    do {                                                                                           \
        const char *p_ = (str);                                                                    \
        if (p_) {                                                                                  \
            size_t n_ = mbstowcs(s, p_, (size_t)(endp - s));                                       \
            if (n_ == (size_t)-1)                                                                  \
                return 0;                                                                          \
            s += n_;                                                                               \
        }                                                                                          \
    } while (0)

#define OUTNUM2(v)                                                                                 \
    do {                                                                                           \
        int val_ = (v);                                                                            \
        if (val_ < 0)                                                                              \
            val_ = -val_;                                                                          \
        if (s + 2 > endp)                                                                          \
            return 0;                                                                              \
        *s++ = (wchar_t)(L'0' + (val_ / 10) % 10);                                                 \
        *s++ = (wchar_t)(L'0' + (val_ % 10));                                                      \
    } while (0)

static int wfmt_int(wchar_t *restrict *restrict s,
                    wchar_t *end,
                    long val,
                    int width,
                    wchar_t pad_char)
{
    wchar_t buf[32];
    wchar_t *p = buf;
    int neg = val < 0;
    if (neg)
        val = -val;
    do {
        *p++ = (wchar_t)(L'0' + (val % 10));
        val /= 10;
        width--;
    } while (val > 0);
    while (width-- > 0)
        *p++ = pad_char;
    if (neg)
        *p++ = L'-';
    while (p > buf) {
        if (*s >= end)
            return 0;
        *(*s)++ = *--p;
    }
    return 1;
}

static int weeknumber(const struct tm *t, int firstw)
{
    int w = t->tm_wday;
    int r;
    if (firstw == 1)
        w = (w == 0) ? 6 : w - 1;
    r = (t->tm_yday + 7 - w) / 7;
    return r < 0 ? 0 : r;
}

static int iso8601wknum(const struct tm *t)
{
    int w = weeknumber(t, 1);
    int j = t->tm_wday - (t->tm_yday % 7);
    if (j < 0)
        j += 7;
    if (j >= 1 && j <= 4)
        w++;
    else if ((j == 5 || j == 6 || j == 0) && w == 0) {
        struct tm p = *t;
        p.tm_year--;
        p.tm_mon = 11;
        p.tm_mday = 31;
        p.tm_wday = (j == 0) ? 6 : j - 1;
        p.tm_yday = 364 + ((p.tm_year + 1900) % 4 == 0 &&
                           ((p.tm_year + 1900) % 100 != 0 || (p.tm_year + 1900) % 400 == 0));
        return iso8601wknum(&p);
    }
    if (t->tm_mon == 11) {
        int wd = t->tm_wday, md = t->tm_mday;
        if ((wd == 1 && md >= 29) || (wd == 2 && md >= 30) || (wd == 3 && md == 31))
            w = 1;
    }
    return w;
}

size_t wcsftime(wchar_t *restrict s,
                size_t maxsize,
                const wchar_t *restrict format,
                const struct tm *restrict timeptr)
{
    wchar_t *start = s;
    wchar_t *endp;
    int tz_set = 0;

    if (!s || !format || !timeptr || maxsize == 0)
        return 0;

    endp = s + maxsize - 1;

    for (; *format; format++) {
        if (*format != L'%') {
            if (s >= endp)
                return 0;
            *s++ = *format;
            continue;
        }
        format++;
    again:
        switch (*format) {
        case L'\0':
            if (s >= endp)
                return 0;
            *s++ = L'%';
            format--;
            break;
        case L'%':
            if (s >= endp)
                return 0;
            *s++ = L'%';
            break;
        case L'a':
            OUTSTR(nl_langinfo(abday_i[RANGE(0, timeptr->tm_wday, 6)]));
            break;
        case L'A':
            OUTSTR(nl_langinfo(day_i[RANGE(0, timeptr->tm_wday, 6)]));
            break;
        case L'b':
        case L'h':
            OUTSTR(nl_langinfo(abmon_i[RANGE(0, timeptr->tm_mon, 11)]));
            break;
        case L'B':
            OUTSTR(nl_langinfo(mon_i[RANGE(0, timeptr->tm_mon, 11)]));
            break;
        case L'c': {
            const char *fmt = nl_langinfo(D_T_FMT);
            if (fmt && *fmt) {
                wchar_t wfmt[256];
                if (mbstowcs(wfmt, fmt, 256) == (size_t)-1)
                    return 0;
                size_t r = wcsftime(s, (size_t)(endp - s + 1), wfmt, timeptr);
                if (!r)
                    return 0;
                s += r;
            }
            break;
        }
        case L'C':
            if (!wfmt_int(&s, endp, (1900L + timeptr->tm_year) / 100, 2, L'0'))
                return 0;
            break;
        case L'd':
            OUTNUM2(RANGE(1, timeptr->tm_mday, 31));
            break;
        case L'D':
            OUTNUM2(RANGE(0, timeptr->tm_mon, 11) + 1);
            if (s >= endp)
                return 0;
            *s++ = L'/';
            OUTNUM2(RANGE(1, timeptr->tm_mday, 31));
            if (s >= endp)
                return 0;
            *s++ = L'/';
            OUTNUM2((timeptr->tm_year % 100 + 100) % 100);
            break;
        case L'e': {
            int v = RANGE(1, timeptr->tm_mday, 31);
            if (v < 10) {
                if (s >= endp)
                    return 0;
                *s++ = L' ';
                if (s >= endp)
                    return 0;
                *s++ = (wchar_t)(L'0' + v);
            } else
                OUTNUM2(v);
            break;
        }
        case L'E':
        case L'O':
            format++;
            goto again;
        case L'F':
            if (!wfmt_int(&s, endp, 1900L + timeptr->tm_year, 4, L'0'))
                return 0;
            if (s >= endp)
                return 0;
            *s++ = L'-';
            OUTNUM2(RANGE(0, timeptr->tm_mon, 11) + 1);
            if (s >= endp)
                return 0;
            *s++ = L'-';
            OUTNUM2(RANGE(1, timeptr->tm_mday, 31));
            break;
        case L'g':
        case L'G': {
            int w = iso8601wknum(timeptr);
            long y = 1900L + timeptr->tm_year;
            if (timeptr->tm_mon == 11 && w == 1)
                y++;
            else if (timeptr->tm_mon == 0 && w >= 52)
                y--;
            if (*format == L'G') {
                if (!wfmt_int(&s, endp, y, 4, L'0'))
                    return 0;
            } else {
                OUTNUM2((y % 100 + 100) % 100);
            }
            break;
        }
        case L'H':
            OUTNUM2(RANGE(0, timeptr->tm_hour, 23));
            break;
        case L'I': {
            int i = RANGE(0, timeptr->tm_hour, 23);
            if (i == 0)
                i = 12;
            else if (i > 12)
                i -= 12;
            OUTNUM2(i);
            break;
        }
        case L'j':
            if (!wfmt_int(&s, endp, timeptr->tm_yday + 1, 3, L'0'))
                return 0;
            break;
        case L'm':
            OUTNUM2(RANGE(0, timeptr->tm_mon, 11) + 1);
            break;
        case L'M':
            OUTNUM2(RANGE(0, timeptr->tm_min, 59));
            break;
        case L'n':
            if (s >= endp)
                return 0;
            *s++ = L'\n';
            break;
        case L'p':
            OUTSTR(nl_langinfo(ampm_i[timeptr->tm_hour < 12 ? 0 : 1]));
            break;
        case L'r': {
            const char *fmt = nl_langinfo(T_FMT_AMPM);
            if (fmt && *fmt) {
                wchar_t wfmt[256];
                if (mbstowcs(wfmt, fmt, 256) == (size_t)-1)
                    return 0;
                size_t r = wcsftime(s, (size_t)(endp - s + 1), wfmt, timeptr);
                if (!r)
                    return 0;
                s += r;
            }
            break;
        }
        case L'R':
            OUTNUM2(RANGE(0, timeptr->tm_hour, 23));
            if (s >= endp)
                return 0;
            *s++ = L':';
            OUTNUM2(RANGE(0, timeptr->tm_min, 59));
            break;
        case L's': {
            struct tm tmp = *timeptr;
            if (!wfmt_int(&s, endp, (long)mktime(&tmp), 1, L'0'))
                return 0;
            break;
        }
        case L'S':
            OUTNUM2(RANGE(0, timeptr->tm_sec, 60));
            break;
        case L't':
            if (s >= endp)
                return 0;
            *s++ = L'\t';
            break;
        case L'T':
        the_time:
            OUTNUM2(RANGE(0, timeptr->tm_hour, 23));
            if (s >= endp)
                return 0;
            *s++ = L':';
            OUTNUM2(RANGE(0, timeptr->tm_min, 59));
            if (s >= endp)
                return 0;
            *s++ = L':';
            OUTNUM2(RANGE(0, timeptr->tm_sec, 60));
            break;
        case L'u': {
            int u = timeptr->tm_wday == 0 ? 7 : timeptr->tm_wday;
            if (s >= endp)
                return 0;
            *s++ = (wchar_t)(L'0' + u);
            break;
        }
        case L'U':
            OUTNUM2(weeknumber(timeptr, 0));
            break;
        case L'V':
            OUTNUM2(iso8601wknum(timeptr));
            break;
        case L'w':
            if (s >= endp)
                return 0;
            *s++ = (wchar_t)(L'0' + RANGE(0, timeptr->tm_wday, 6));
            break;
        case L'W':
            OUTNUM2(weeknumber(timeptr, 1));
            break;
        case L'x': {
            const char *fmt = nl_langinfo(D_FMT);
            if (fmt && *fmt) {
                wchar_t wfmt[256];
                if (mbstowcs(wfmt, fmt, 256) == (size_t)-1)
                    return 0;
                size_t r = wcsftime(s, (size_t)(endp - s + 1), wfmt, timeptr);
                if (!r)
                    return 0;
                s += r;
            }
            break;
        }
        case L'X': {
            const char *fmt = nl_langinfo(T_FMT);
            if (fmt && *fmt) {
                wchar_t wfmt[256];
                if (mbstowcs(wfmt, fmt, 256) == (size_t)-1)
                    return 0;
                size_t r = wcsftime(s, (size_t)(endp - s + 1), wfmt, timeptr);
                if (!r)
                    return 0;
                s += r;
            } else {
                goto the_time;
            }
            break;
        }
        case L'y':
            OUTNUM2((timeptr->tm_year % 100 + 100) % 100);
            break;
        case L'Y':
            if (!wfmt_int(&s, endp, 1900L + timeptr->tm_year, 4, L'0'))
                return 0;
            break;
        case L'z': {
            long off;
            if (timeptr->tm_isdst < 0)
                break;
            if (!tz_set) {
                tzset();
                tz_set = 1;
            }
            off = -timezone;
            if (timeptr->tm_isdst > 0)
                off += 3600;
            off /= 60;
            if (off < 0) {
                if (s >= endp)
                    return 0;
                *s++ = L'-';
                off = -off;
            } else {
                if (s >= endp)
                    return 0;
                *s++ = L'+';
            }
            OUTNUM2(off / 60);
            OUTNUM2(off % 60);
            break;
        }
        case L'Z': {
            if (timeptr->tm_isdst < 0)
                break;
            if (!tz_set) {
                tzset();
                tz_set = 1;
            }
            OUTSTR(tzname[timeptr->tm_isdst > 0 ? 1 : 0]);
            break;
        }
        default:
            if (s + 2 > endp)
                return 0;
            *s++ = L'%';
            *s++ = *format;
            break;
        }
    }
    *s = L'\0';
    return (size_t)(s - start);
}
