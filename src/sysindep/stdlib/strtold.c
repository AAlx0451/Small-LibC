#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>

long double strtold(const char *restrict nptr, char **restrict endptr) {
    const char *p = nptr;
    long double value = 0.0L;
    int sign = 1;
    long double scale;
    int exp_sign = 1;
    long exponent = 0;
    long decimal_exp = 0;
    int has_digits = 0;

    while(isspace((unsigned char)*p)) {
        p++;
    }

    if(*p == '-') {
        sign = -1;
        p++;
    } else if(*p == '+') {
        p++;
    }

    if((tolower((unsigned char)p[0]) == 'i') && (tolower((unsigned char)p[1]) == 'n') && (tolower((unsigned char)p[2]) == 'f')) {
        p += 3;
        if((tolower((unsigned char)p[0]) == 'i') && (tolower((unsigned char)p[1]) == 'n') && (tolower((unsigned char)p[2]) == 'i') &&
           (tolower((unsigned char)p[3]) == 't') && (tolower((unsigned char)p[4]) == 'y')) {
            p += 5;
        }
        if(endptr)
            *endptr = (char *)p;
        return sign * (1.0L / 0.0L);
    }

    if((tolower((unsigned char)p[0]) == 'n') && (tolower((unsigned char)p[1]) == 'a') && (tolower((unsigned char)p[2]) == 'n')) {
        p += 3;
        if(endptr)
            *endptr = (char *)p;
        return 0.0L / 0.0L;
    }

    while(isdigit((unsigned char)*p)) {
        has_digits = 1;
        value = value * 10.0L + (*p - '0');
        p++;
    }

    if(*p == '.') {
        p++;
        while(isdigit((unsigned char)*p)) {
            has_digits = 1;
            value = value * 10.0L + (*p - '0');
            decimal_exp--;
            p++;
        }
    }

    if(!has_digits) {
        if(endptr)
            *endptr = (char *)nptr;
        return 0.0L;
    }

    if(tolower((unsigned char)*p) == 'e') {
        p++;
        if(*p == '-') {
            exp_sign = -1;
            p++;
        } else if(*p == '+') {
            p++;
        }

        const char *save_p = p;
        int exp_has_digits = 0;
        long tmp_exp = 0;

        while(isdigit((unsigned char)*p)) {
            exp_has_digits = 1;
            if(tmp_exp < 100000) {
                tmp_exp = tmp_exp * 10 + (*p - '0');
            }
            p++;
        }

        if(!exp_has_digits) {
            p = save_p - 1;
        } else {
            exponent = tmp_exp * exp_sign;
        }
    }

    exponent += decimal_exp;

    if(value != 0.0L) {
        scale = 1.0L;
        long abs_exp = (exponent < 0) ? -exponent : exponent;
        long double p10 = 10.0L;

        if(abs_exp > 5000) {
            if(exponent > 0) {
                errno = ERANGE;
                value = 1.0L / 0.0L;
            } else {
                errno = ERANGE;
                value = 0.0L;
            }
        } else {
            while(abs_exp > 0) {
                if(abs_exp & 1) {
                    scale *= p10;
                }
                p10 *= p10;
                abs_exp >>= 1;
            }

            if(exponent < 0) {
                value /= scale;
            } else {
                value *= scale;
            }
        }
    }

    if(value > LDBL_MAX) {
        errno = ERANGE;
        value = (sign == 1) ? LDBL_MAX : -LDBL_MAX;
    } else {
        value *= sign;
    }

    if(endptr) {
        *endptr = (char *)p;
    }

    return value;
}
