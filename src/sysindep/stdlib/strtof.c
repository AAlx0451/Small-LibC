#include <stdlib.h>

float strtof(const char *restrict nptr, char **restrict endptr) {
    return (float)strtod(nptr, endptr);
}
