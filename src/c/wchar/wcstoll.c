#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <wchar.h>
#include <wctype.h>

long long wcstoll(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base) {
    const wchar_t *s = nptr;
    unsigned long long acc;
    wint_t c;
    unsigned long long cutoff, cutlim;
    int neg = 0, any;

    do {
        c = *s++;
    } while(iswspace(c));

    if(c == L'-') {
        neg = 1;
        c = *s++;
    } else if(c == L'+') {
        c = *s++;
    }

    if((base == 0 || base == 16) && c == L'0' && (*s == L'x' || *s == L'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if(base == 0) {
        base = c == L'0' ? 8 : 10;
    }

    cutoff = neg ? -(unsigned long long)LLONG_MIN : LLONG_MAX;
    cutlim = cutoff % (unsigned long long)base;
    cutoff /= (unsigned long long)base;

    for(acc = 0, any = 0;; c = *s++) {
        if(c >= L'0' && c <= L'9')
            c -= L'0';
        else if(c >= L'A' && c <= L'Z')
            c -= L'A' - 10;
        else if(c >= L'a' && c <= L'z')
            c -= L'a' - 10;
        else
            break;

        if((int)c >= base)
            break;

        if(any < 0 || acc > cutoff || (acc == cutoff && (unsigned long long)c > cutlim)) {
            any = -1;
        } else {
            any = 1;
            acc *= (unsigned long long)base;
            acc += (unsigned long long)c;
        }
    }

    if(any < 0) {
        acc = neg ? (unsigned long long)LLONG_MIN : (long long)LLONG_MAX;
        errno = ERANGE;
    } else if(neg) {
        acc = -acc;
    }

    if(endptr != 0) {
        *endptr = __deconst(wchar_t *, (any ? s - 1 : nptr));
    }

    return (long long)acc;
}
