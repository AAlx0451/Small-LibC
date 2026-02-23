#include <errno.h>
#include <limits.h>
#include <stdlib.h>

unsigned long strtoul(const char *restrict nptr, char **restrict endptr, int base) {
    unsigned long long val;
    val = strtoull(nptr, endptr, base);

    if(val > ULONG_MAX) {
        errno = ERANGE;
        return ULONG_MAX;
    }

    return (unsigned long)val;
}
