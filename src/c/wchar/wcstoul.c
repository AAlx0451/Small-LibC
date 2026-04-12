#include <errno.h>
#include <limits.h>
#include <wchar.h>

long wcstol(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base) {
    long long val = wcstoll(nptr, endptr, base);
    if(val > LONG_MAX) {
        if(endptr)
            wcstoll(nptr, endptr, base);
        errno = ERANGE;
        return LONG_MAX;
    }
    if(val < LONG_MIN) {
        if(endptr)
            wcstoll(nptr, endptr, base);
        errno = ERANGE;
        return LONG_MIN;
    }
    return (long)val;
}
