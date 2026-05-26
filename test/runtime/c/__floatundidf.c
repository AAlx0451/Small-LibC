#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define GREEN(txt) "\033[0;32m" txt "\033[0m"
#define RED(txt)   "\033[0;31m" txt "\033[0m"

#define undidf(a) __floatundidf(a)

#define ULLONG_MAX_AS_DOUBLE 18446744073709551616.0

#define MSB_SET_AS_DOUBLE 9223372036854775808.0

void __floatundidf_test()
{
    if (undidf(1ULL) != 1.0)
        printf(RED("[FAIL]") " (double)1ULL == 1.0\n");
    else
        printf(GREEN("[PASS]") " (double)1ULL == 1.0\n");

    if (undidf(1ULL << 63) != MSB_SET_AS_DOUBLE)
        printf(RED("[FAIL]") " (double)(1ULL<<63) == 9223372036854775808.0\n");
    else
        printf(GREEN("[PASS]") " (double)(1ULL<<63) == 9223372036854775808.0\n");

    if (undidf(ULLONG_MAX) != ULLONG_MAX_AS_DOUBLE)
        printf(RED("[FAIL]") " (double)ULLONG_MAX == (2.0^64)-1\n");
    else
        printf(GREEN("[PASS]") " (double)ULLONG_MAX == (2.0^64)-1\n");
}
