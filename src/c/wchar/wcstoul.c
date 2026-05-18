#include <errno.h>
#include <limits.h>
#include <wchar.h>

unsigned long wcstoul(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base) {
    unsigned long long val = wcstoull(nptr, endptr, base);
    if(val > ULONG_MAX) {
        if(endptr)
            wcstoull(nptr, endptr, base);
        errno = ERANGE;
        return ULONG_MAX;
    }
    return (unsigned long)val;
}
