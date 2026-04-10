#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH "/usr/share/locale"

static struct lconv current_lconv;
static char *num_buf = NULL;
static char *mon_buf = NULL;
static char last_num_loc[256] = "";
static char last_mon_loc[256] = "";
static char grouping_bin[16];
static char mon_grouping_bin[16];

static char *parse_grouping(char *src, char *dest) {
    int i = 0;
    char *token = strtok(src, ";");
    while(token && i < 15) {
        int val = atoi(token);
        if(val <= 0)
            break;
        dest[i++] = (char)val;
        token = strtok(NULL, ";");
    }
    dest[i] = '\0';
    return dest;
}

static char *get_str_val(char **ptr, char *end) {
    char *p = *ptr;
    if(p >= end)
        return "";
    *ptr = p + strlen(p) + 1;
    return p;
}

static char get_int_val(char **ptr, char *end) {
    char *p = get_str_val(ptr, end);
    if(*p == '\0' || strcmp(p, "-1") == 0)
        return CHAR_MAX;
    return (char)atoi(p);
}

static char *load_file(const char *loc, const char *cat, long *out_sz) {
    char path[512];
    FILE *f;
    long sz, i;
    char *buf;

    if(strcmp(loc, "C") == 0 || strcmp(loc, "POSIX") == 0)
        return NULL;

    snprintf(path, sizeof(path), "%s/%s/%s", PATH, loc, cat);
    f = fopen(path, "rb");
    if(!f)
        return NULL;

    fseek(f, 0, SEEK_END);
    sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    buf = malloc(sz + 1);
    if(buf) {
        fread(buf, 1, sz, f);
        buf[sz] = '\0';
        for(i = 0; i < sz; i++) {
            if(buf[i] == '\n')
                buf[i] = '\0';
        }
        *out_sz = sz;
    }
    fclose(f);
    return buf;
}

static void reset_numeric(void) {
    current_lconv.decimal_point = ".";
    current_lconv.thousands_sep = "";
    current_lconv.grouping = "";
}

static void reset_monetary(void) {
    current_lconv.int_curr_symbol = "";
    current_lconv.currency_symbol = "";
    current_lconv.mon_decimal_point = "";
    current_lconv.mon_thousands_sep = "";
    current_lconv.mon_grouping = "";
    current_lconv.positive_sign = "";
    current_lconv.negative_sign = "";
    current_lconv.int_frac_digits = CHAR_MAX;
    current_lconv.frac_digits = CHAR_MAX;
    current_lconv.p_cs_precedes = CHAR_MAX;
    current_lconv.p_sep_by_space = CHAR_MAX;
    current_lconv.n_cs_precedes = CHAR_MAX;
    current_lconv.n_sep_by_space = CHAR_MAX;
    current_lconv.p_sign_posn = CHAR_MAX;
    current_lconv.n_sign_posn = CHAR_MAX;
}

struct lconv *localeconv(void) {
    char *loc_num = setlocale(LC_NUMERIC, NULL);
    char *loc_mon = setlocale(LC_MONETARY, NULL);
    long sz = 0;
    char *p, *end;

    if(!loc_num)
        loc_num = "C";
    if(!loc_mon)
        loc_mon = "C";

    if(strcmp(loc_num, last_num_loc) != 0) {
        strncpy(last_num_loc, loc_num, sizeof(last_num_loc) - 1);
        if(num_buf)
            free(num_buf);
        num_buf = load_file(loc_num, "LC_NUMERIC", &sz);
        if(num_buf) {
            p = num_buf;
            end = num_buf + sz;
            current_lconv.decimal_point = get_str_val(&p, end);
            current_lconv.thousands_sep = get_str_val(&p, end);
            char *raw_grp = get_str_val(&p, end);
            current_lconv.grouping = parse_grouping(raw_grp, grouping_bin);
        } else {
            reset_numeric();
        }
    }

    if(strcmp(loc_mon, last_mon_loc) != 0) {
        strncpy(last_mon_loc, loc_mon, sizeof(last_mon_loc) - 1);
        if(mon_buf)
            free(mon_buf);
        mon_buf = load_file(loc_mon, "LC_MONETARY", &sz);
        if(mon_buf) {
            p = mon_buf;
            end = mon_buf + sz;
            current_lconv.int_curr_symbol = get_str_val(&p, end);
            current_lconv.currency_symbol = get_str_val(&p, end);
            current_lconv.mon_decimal_point = get_str_val(&p, end);
            current_lconv.mon_thousands_sep = get_str_val(&p, end);
            char *raw_mon_grp = get_str_val(&p, end);
            current_lconv.mon_grouping = parse_grouping(raw_mon_grp, mon_grouping_bin);
            current_lconv.positive_sign = get_str_val(&p, end);
            current_lconv.negative_sign = get_str_val(&p, end);
            current_lconv.int_frac_digits = get_int_val(&p, end);
            current_lconv.frac_digits = get_int_val(&p, end);
            current_lconv.p_cs_precedes = get_int_val(&p, end);
            current_lconv.p_sep_by_space = get_int_val(&p, end);
            current_lconv.n_cs_precedes = get_int_val(&p, end);
            current_lconv.n_sep_by_space = get_int_val(&p, end);
            current_lconv.p_sign_posn = get_int_val(&p, end);
            current_lconv.n_sign_posn = get_int_val(&p, end);
        } else {
            reset_monetary();
        }
    }

    return &current_lconv;
}
