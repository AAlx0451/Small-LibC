#include <langinfo.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char __nl_buf[512];

static char *__get_line_from_file(const char *category, const char *loc_name, int line_idx) {
    char path[256];
    snprintf(path, sizeof(path), "/usr/share/locale/%s/%s", loc_name, category);

    FILE *f = fopen(path, "r");
    if(!f)
        return "";

    int cur = 0;
    while(fgets(__nl_buf, sizeof(__nl_buf), f)) {
        if(cur == line_idx) {
            char *nl = strchr(__nl_buf, '\n');
            if(nl)
                *nl = '\0';
            fclose(f);
            return __nl_buf;
        }
        cur++;
    }
    fclose(f);
    return "";
}

char *nl_langinfo(nl_item item) {
    if(item == CODESET) {
        char *loc = setlocale(LC_ALL, NULL);
        if(!loc || !strcmp(loc, "C") || !strcmp(loc, "POSIX"))
            return "ASCII";
        char *dot = strchr(loc, '.');
        return dot ? dot + 1 : "UTF-8";
    }

    if(item == RADIXCHAR)
        return localeconv()->decimal_point;
    if(item == THOUSEP)
        return localeconv()->thousands_sep;
    if(item == CRNCYSTR)
        return localeconv()->currency_symbol;

    if(item >= ABMON_1 && item <= T_FMT_AMPM) {
        char *loc = setlocale(LC_TIME, NULL);
        if(!loc || !strcmp(loc, "C") || !strcmp(loc, "POSIX"))
            loc = "en_US";
        int line = 0;
        if(item <= ABMON_12)
            line = item - ABMON_1;
        else if(item <= MON_12)
            line = 12 + (item - MON_1);
        else if(item <= ABDAY_7)
            line = 24 + (item - ABDAY_1);
        else if(item <= DAY_7)
            line = 31 + (item - DAY_1);
        else if(item == T_FMT)
            line = 38;
        else if(item == D_FMT)
            line = 39;
        else if(item == D_T_FMT)
            line = 40;
        else if(item == AM_STR)
            line = 41;
        else if(item == PM_STR)
            line = 42;
        else if(item == T_FMT_AMPM)
            line = 57;

        return __get_line_from_file("LC_TIME", loc, line);
    }

    if(item == YESEXPR || item == NOEXPR) {
        char *loc = setlocale(LC_MESSAGES, NULL);
        if(!loc || !strcmp(loc, "C") || !strcmp(loc, "POSIX"))
            loc = "en_US";
        return __get_line_from_file("LC_MESSAGES", loc, item == YESEXPR ? 0 : 1);
    }

    return "";
}
