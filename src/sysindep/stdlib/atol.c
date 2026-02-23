#include <ctype.h>

long atol(const char *nptr) {
    long result = 0;
    long sign = 1;
    while(isspace((unsigned char)*nptr)) {
        nptr++;
    }
    if(*nptr == '-') {
        sign = -1;
        nptr++;
    } else if(*nptr == '+') {
        nptr++;
    }
    while(isdigit((unsigned char)*nptr)) {
        result = result * 10 + (*nptr - '0');
        nptr++;
    }
    return result * sign;
}
