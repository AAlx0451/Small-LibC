#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

long double strtold(const char *restrict nptr, char **restrict endptr)
{
    const char *p = nptr;
    const char *save_p;
    long double value = 0.0L, scale;
    int sign = 1, exp_sign = 1, exp_has_digits, tmp_exp, has_digits = 0;
    long exponent = 0, radix_exp = 0, abs_exp;
    int is_hex = 0;
    const char separ = localeconv()->decimal_point[0];

    while (isspace((unsigned char)*p)) {
        p++;
    }

    if (*p == '-') {
        sign = -1;
        p++;
    } else if (*p == '+') {
        p++;
    }

    if ((tolower((unsigned char)p[0]) == 'i') && (tolower((unsigned char)p[1]) == 'n') &&
        (tolower((unsigned char)p[2]) == 'f')) {
        p += 3;
        if ((tolower((unsigned char)p[0]) == 'i') && (tolower((unsigned char)p[1]) == 'n') &&
            (tolower((unsigned char)p[2]) == 'i') && (tolower((unsigned char)p[3]) == 't') &&
            (tolower((unsigned char)p[4]) == 'y')) {
            p += 5;
        }
        if (endptr)
            *endptr = __deconst(char *, p);
        return sign * HUGE_VALL;
    }

    if ((tolower((unsigned char)p[0]) == 'n') && (tolower((unsigned char)p[1]) == 'a') &&
        (tolower((unsigned char)p[2]) == 'n')) {
        p += 3;
        if (endptr)
            *endptr = __deconst(char *, p);
        return (long double)NAN;
    }

    if (p[0] == '0' && tolower((unsigned char)p[1]) == 'x') {
        int has_hex_digit = isxdigit((unsigned char)p[2]);
        if (!has_hex_digit && p[2] == separ) {
            has_hex_digit = isxdigit((unsigned char)p[3]);
        }
        if (has_hex_digit) {
            is_hex = 1;
            p += 2;
        }
    }

    while (1) {
        int c = (unsigned char)*p;
        int digit = -1;
        if (isdigit(c)) {
            digit = c - '0';
        } else if (is_hex && isalpha(c)) {
            int lc = tolower(c);
            if (lc >= 'a' && lc <= 'f') {
                digit = lc - 'a' + 10;
            }
        }
        if (digit == -1)
            break;
        has_digits = 1;
        value = value * (is_hex ? 16.0L : 10.0L) + digit;
        p++;
    }

    if (*p == separ) {
        p++;
        while (1) {
            int c = (unsigned char)*p;
            int digit = -1;
            if (isdigit(c)) {
                digit = c - '0';
            } else if (is_hex && isalpha(c)) {
                int lc = tolower(c);
                if (lc >= 'a' && lc <= 'f') {
                    digit = lc - 'a' + 10;
                }
            }
            if (digit == -1)
                break;
            has_digits = 1;
            value = value * (is_hex ? 16.0L : 10.0L) + digit;
            radix_exp--;
            p++;
        }
    }

    if (!has_digits) {
        if (endptr)
            *endptr = (char *)(uintptr_t)nptr;
        return 0.0L;
    }

    char exp_char = is_hex ? 'p' : 'e';
    if (tolower((unsigned char)*p) == exp_char) {
        p++;
        if (*p == '-') {
            exp_sign = -1;
            p++;
        } else if (*p == '+') {
            p++;
        }
        save_p = p;
        exp_has_digits = 0;
        tmp_exp = 0;
        while (isdigit((unsigned char)*p)) {
            exp_has_digits = 1;
            if (tmp_exp < 100000) {
                tmp_exp = tmp_exp * 10 + (*p - '0');
            }
            p++;
        }
        if (!exp_has_digits) {
            p = save_p - 1;
        } else {
            exponent = tmp_exp * exp_sign;
        }
    }

    if (value != 0.0L) {
        scale = 1.0L;
        if (is_hex) {
            long exp2 = exponent + (radix_exp * 4);
            abs_exp = (exp2 < 0) ? -exp2 : exp2;
            long double p2 = 2.0L;
            if (abs_exp > 20000) {
                if (exp2 > 0) {
                    errno = ERANGE;
                    value = 1.0L / 0.0L;
                } else {
                    errno = ERANGE;
                    value = 0.0L;
                }
            } else {
                while (abs_exp > 0) {
                    if (abs_exp & 1)
                        scale *= p2;
                    p2 *= p2;
                    abs_exp >>= 1;
                }
                if (exp2 < 0)
                    value /= scale;
                else
                    value *= scale;
            }
        } else {
            long exp10 = exponent + radix_exp;
            abs_exp = (exp10 < 0) ? -exp10 : exp10;
            long double p10 = 10.0L;
            if (abs_exp > 5000) {
                if (exp10 > 0) {
                    errno = ERANGE;
                    value = HUGE_VALL;
                } else {
                    errno = ERANGE;
                    value = 0.0L;
                }
            } else {
                while (abs_exp > 0) {
                    if (abs_exp & 1)
                        scale *= p10;
                    p10 *= p10;
                    abs_exp >>= 1;
                }
                if (exp10 < 0)
                    value /= scale;
                else
                    value *= scale;
            }
        }
    }

    if (value > LDBL_MAX) {
        errno = ERANGE;
        value = (sign == 1) ? LDBL_MAX : -LDBL_MAX;
    } else {
        value *= sign;
    }

    if (endptr) {
        *endptr = __deconst(char *, p);
    }

    return value;
}
