#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

long double wcstold(const wchar_t *restrict nptr, wchar_t **restrict endptr)
{
    const wchar_t *p = nptr;
    const wchar_t *save_p;
    long double value = 0.0L, scale;
    int sign = 1, exp_sign = 1, exp_has_digits, tmp_exp, has_digits = 0;
    long exponent = 0, radix_exp = 0, abs_exp;
    int is_hex = 0;

    const wchar_t separ = (wchar_t)(unsigned char)localeconv()->decimal_point[0];

    while (iswspace(*p)) {
        p++;
    }

    if (*p == L'-') {
        sign = -1;
        p++;
    } else if (*p == L'+') {
        p++;
    }

    if ((towlower(p[0]) == L'i') && (towlower(p[1]) == L'n') && (towlower(p[2]) == L'f')) {
        p += 3;
        if ((towlower(p[0]) == L'i') && (towlower(p[1]) == L'n') && (towlower(p[2]) == L'i') &&
            (towlower(p[3]) == L't') && (towlower(p[4]) == L'y')) {
            p += 5;
        }
        if (endptr)
            *endptr = __deconst(wchar_t *, p);
        return sign * (1.0L / 0.0L);
    }

    if ((towlower(p[0]) == L'n') && (towlower(p[1]) == L'a') && (towlower(p[2]) == L'n')) {
        p += 3;
        if (endptr)
            *endptr = __deconst(wchar_t *, p);
        return 0.0L / 0.0L;
    }

    if (p[0] == L'0' && towlower(p[1]) == L'x') {
        int has_hex_digit = iswxdigit(p[2]);
        if (!has_hex_digit && p[2] == separ) {
            has_hex_digit = iswxdigit(p[3]);
        }
        if (has_hex_digit) {
            is_hex = 1;
            p += 2;
        }
    }

    while (1) {
        wchar_t c = *p;
        int digit = -1;
        if (iswdigit(c)) {
            digit = c - L'0';
        } else if (is_hex && iswalpha(c)) {
            wchar_t lc = towlower(c);
            if (lc >= L'a' && lc <= L'f') {
                digit = lc - L'a' + 10;
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
            wchar_t c = *p;
            int digit = -1;
            if (iswdigit(c)) {
                digit = c - L'0';
            } else if (is_hex && iswalpha(c)) {
                wchar_t lc = towlower(c);
                if (lc >= L'a' && lc <= L'f') {
                    digit = lc - L'a' + 10;
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
            *endptr = __deconst(wchar_t *, nptr);
        return 0.0L;
    }

    wchar_t exp_char = is_hex ? L'p' : L'e';
    if (towlower(*p) == exp_char) {
        p++;
        if (*p == L'-') {
            exp_sign = -1;
            p++;
        } else if (*p == L'+') {
            p++;
        }
        save_p = p;
        exp_has_digits = 0;
        tmp_exp = 0;
        while (iswdigit(*p)) {
            exp_has_digits = 1;
            if (tmp_exp < 100000) {
                tmp_exp = tmp_exp * 10 + (*p - L'0');
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
                    value = 1.0L / 0.0L;
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
        *endptr = __deconst(wchar_t *, p);
    }

    return value;
}
