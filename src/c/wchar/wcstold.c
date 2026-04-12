#include <wctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

long double wcstold(const wchar_t *restrict nptr, wchar_t **restrict endptr) {
    const wchar_t *p = nptr;
    wchar_t *save_p;
    long double value = 0.0L, scale, p10;
    int sign = 1, exp_sign = 1, exp_has_digits, tmp_exp, has_digits = 0;
    long exponent = 0, decimal_exp = 0, abs_exp;
    
    /* Safely cast the narrow decimal point to wide character */
    const wchar_t separ = (wchar_t)(unsigned char)localeconv()->decimal_point[0];

    while(iswspace(*p)) {
        p++;
    }

    if(*p == L'-') {
        sign = -1;
        p++;
    } else if(*p == L'+') {
        p++;
    }

    if((towlower(p[0]) == L'i') && (towlower(p[1]) == L'n') && (towlower(p[2]) == L'f')) {
        p += 3;
        if((towlower(p[0]) == L'i') && (towlower(p[1]) == L'n') && (towlower(p[2]) == L'i') &&
           (towlower(p[3]) == L't') && (towlower(p[4]) == L'y')) {
            p += 5;
        }
        if(endptr)
            *endptr = (wchar_t *)(uintptr_t)p;
        return sign * (1.0L / 0.0L);
    }

    if((towlower(p[0]) == L'n') && (towlower(p[1]) == L'a') && (towlower(p[2]) == L'n')) {
        p += 3;
        if(endptr)
            *endptr = (wchar_t *)(uintptr_t)p;
        return 0.0L / 0.0L;
    }

    while(iswdigit(*p)) {
        has_digits = 1;
        value = value * 10.0L + (*p - L'0');
        p++;
    }

    if(*p == separ) {
        p++;
        while(iswdigit(*p)) {
            has_digits = 1;
            value = value * 10.0L + (*p - L'0');
            decimal_exp--;
            p++;
        }
    }

    if(!has_digits) {
        if(endptr)
            *endptr = (wchar_t *)(uintptr_t)nptr;
        return 0.0L;
    }

    if(towlower(*p) == L'e') {
        p++;
        if(*p == L'-') {
            exp_sign = -1;
            p++;
        } else if(*p == L'+') {
            p++;
        }

        save_p = (wchar_t *)(uintptr_t)p;
        exp_has_digits = 0;
        tmp_exp = 0;

        while(iswdigit(*p)) {
            exp_has_digits = 1;
            if(tmp_exp < 100000) {
                tmp_exp = tmp_exp * 10 + (*p - L'0');
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
        abs_exp = (exponent < 0) ? -exponent : exponent;
        p10 = 10.0L;

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
        *endptr = (wchar_t *)(uintptr_t)p;
    }

    return value;
}
