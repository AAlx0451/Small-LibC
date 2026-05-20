#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

#define umod(a, b) __umoddi3(a, b)

void __umoddi3_test()
{
    if (umod(68719476739ULL, 2ULL) != 1ULL)
        printf(RED("[FAIL]") " pos/pos unsigned 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/pos unsigned 64bit modulo\n");

    if (umod(68719476739ULL, 1ULL) != 0ULL)
        printf(RED("[FAIL]") " pos/1 unsigned 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/1 unsigned 64bit modulo\n");

    if (umod(68719476739ULL, 68719476739ULL) != 0ULL)
        printf(RED("[FAIL]") " pos/self unsigned 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/self unsigned 64bit modulo\n");

    if (umod(ULLONG_MAX, 1ULL) != 0ULL)
        printf(RED("[FAIL]") " ULLONG_MAX/1 unsigned 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " ULLONG_MAX/1 unsigned 64bit modulo\n");

    if (umod(ULLONG_MAX, ULLONG_MAX) != 0ULL)
        printf(RED("[FAIL]") " ULLONG_MAX/ULLONG_MAX unsigned 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " ULLONG_MAX/ULLONG_MAX unsigned 64bit modulo\n");

    if (umod(ULLONG_MAX, 2ULL) != 1ULL)
        printf(RED("[FAIL]") " ULLONG_MAX/2 unsigned 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " ULLONG_MAX/2 unsigned 64bit modulo\n");

    if (umod(ULLONG_MAX, 3ULL) != (ULLONG_MAX % 3ULL))
        printf(RED("[FAIL]") " ULLONG_MAX/3 unsigned 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " ULLONG_MAX/3 unsigned 64bit modulo\n");
}
