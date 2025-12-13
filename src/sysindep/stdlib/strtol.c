#include <stdlib.h>
#include <limits.h>
#include <errno.h>

long strtol(const char *restrict nptr, char **restrict endptr, int base) {
    long long val;
    val = strtoll(nptr, endptr, base);
    
    if (val > LONG_MAX) {
        errno = ERANGE;
        return LONG_MAX;
    }
    if (val < LONG_MIN) {
        errno = ERANGE;
        return LONG_MIN;
    }
    
    return (long)val;
}
