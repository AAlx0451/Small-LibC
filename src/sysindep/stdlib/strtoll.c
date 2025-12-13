#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * Convert string to long long integer.
 *
 * Ignores leading spaces.
 * Handles sign (+/-).
 * Handles bases 2-36.
 * Handles base 0 (auto-detect: 0x... -> hex, 0... -> octal, else decimal).
 * Checks for overflow/underflow (sets ERANGE).
 */

long long strtoll(const char *restrict nptr, char **restrict endptr, int base) {
    const char *s = nptr;
    unsigned long long acc;
    int c;
    unsigned long long cutoff;
    int neg = 0, any, cutlim;
    do {
        c = *s++;
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else if (c == '+') {
        c = *s++;
    }
    if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0) {
        base = c == '0' ? 8 : 10;
    }
    cutoff = neg ? -(unsigned long long)LLONG_MIN : LLONG_MAX;
    cutlim = cutoff % (unsigned long long)base;
    cutoff /= (unsigned long long)base;
    for (acc = 0, any = 0;; c = *s++) {
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;

        if (c >= base)
            break;

        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
            any = -1;
        } else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }

    if (any < 0) {
        acc = neg ? LLONG_MIN : LLONG_MAX;
        errno = ERANGE;
    } else if (neg) {
        acc = -acc;
    }

    if (endptr != 0) {
        *endptr = (char *)(any ? s - 1 : nptr);
    }

    return (long long)acc;
}
