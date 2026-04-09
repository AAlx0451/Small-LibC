#include <ctype.h>
#include <stdlib.h>

long atol(const char *nptr) {
    return (int)strtol(nptr, (char **)NULL, 10);
}
