#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define GREEN(txt) "\033[0;32m" txt "\033[0m"
#define RED(txt) "\033[0;31m" txt "\033[0m"

#define didf(a) __floatdidf(a)

#define LLONG_MAX_AS_DOUBLE 9223372036854775808.0
#define LLONG_MIN_AS_DOUBLE -9223372036854775808.0

void __floatdidf_test()
{
    if (didf(-1LL) != -1.0)
        printf(RED("[FAIL]") " (double)-1 == -1.0\n");
    else
        printf(GREEN("[PASS]") " (double)-1 == -1.0\n");

    if (didf(LLONG_MAX) != LLONG_MAX_AS_DOUBLE)
        printf(RED("[FAIL]") " (double)LLONG_MAX == (2^63)-1\n");
    else
        printf(GREEN("[PASS]") " (double)LLONG_MAX == (2.0^63)-1\n");

    if (didf(LLONG_MIN) != LLONG_MIN_AS_DOUBLE)
        printf(RED("[FAIL]") " (double)LLONG_MIN == -2.0^63\n");
    else
        printf(GREEN("[PASS]") " (double)LLONG_MIN == -2.0^63\n");
}
