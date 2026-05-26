#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

#define udiv(a, b) __udivdi3(a, b)

void __udivdi3_test()
{
    if (udiv(68719476736ULL, 2ULL) != 34359738368ULL)
        printf(RED("[FAIL]") " pos/pos unsigned 64bit division\n");
    else
        printf(GREEN("[PASS]") " pos/pos unsigned 64bit division\n");

    if (udiv(68719476736ULL, 1ULL) != 68719476736ULL)
        printf(RED("[FAIL]") " pos/1 unsigned 64bit division\n");
    else
        printf(GREEN("[PASS]") " pos/1 unsigned 64bit division\n");

    if (udiv(68719476736ULL, 68719476736ULL) != 1ULL)
        printf(RED("[FAIL]") " pos/self unsigned 64bit division\n");
    else
        printf(GREEN("[PASS]") " pos/self unsigned 64bit division\n");

    if (udiv(ULLONG_MAX, 1ULL) != ULLONG_MAX)
        printf(RED("[FAIL]") " ULLONG_MAX/1 unsigned 64bit division\n");
    else
        printf(GREEN("[PASS]") " ULLONG_MAX/1 unsigned 64bit division\n");

    if (udiv(ULLONG_MAX, ULLONG_MAX) != 1ULL)
        printf(RED("[FAIL]") " ULLONG_MAX/ULLONG_MAX unsigned 64bit division\n");
    else
        printf(GREEN("[PASS]") " ULLONG_MAX/ULLONG_MAX unsigned 64bit division\n");

    if (udiv(ULLONG_MAX, 2ULL) != 9223372036854775807ULL)
        printf(RED("[FAIL]") " ULLONG_MAX/2 unsigned 64bit division\n");
    else
        printf(GREEN("[PASS]") " ULLONG_MAX/2 unsigned 64bit division\n");

    if (udiv(7ULL, 3ULL) != 2ULL)
        printf(RED("[FAIL]") " unsigned 64bit division rounding to zero\n");
    else
        printf(GREEN("[PASS]") " unsigned 64bit division rounding to zero\n");
}
