#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

#define udiv(a, b) __udivsi3(a, b)

void __udivsi3_test()
{
    if (udiv(100U, 2U) != 50U)
        printf(RED("[FAIL]") " pos/pos unsigned 32bit division\n");
    else
        printf(GREEN("[PASS]") " pos/pos unsigned 32bit division\n");

    if (udiv(100U, 1U) != 100U)
        printf(RED("[FAIL]") " pos/1 unsigned 32bit division\n");
    else
        printf(GREEN("[PASS]") " pos/1 unsigned 32bit division\n");

    if (udiv(100U, 100U) != 1U)
        printf(RED("[FAIL]") " pos/self unsigned 32bit division\n");
    else
        printf(GREEN("[PASS]") " pos/self unsigned 32bit division\n");

    if (udiv(UINT_MAX, 1U) != UINT_MAX)
        printf(RED("[FAIL]") " UINT_MAX/1 unsigned 32bit division\n");
    else
        printf(GREEN("[PASS]") " UINT_MAX/1 unsigned 32bit division\n");

    if (udiv(UINT_MAX, UINT_MAX) != 1U)
        printf(RED("[FAIL]") " UINT_MAX/UINT_MAX unsigned 32bit division\n");
    else
        printf(GREEN("[PASS]") " UINT_MAX/UINT_MAX unsigned 32bit division\n");

    if (udiv(UINT_MAX, 2U) != 2147483647U)
        printf(RED("[FAIL]") " UINT_MAX/2 unsigned 32bit division\n");
    else
        printf(GREEN("[PASS]") " UINT_MAX/2 unsigned 32bit division\n");

    if (udiv(7U, 3U) != 2U)
        printf(RED("[FAIL]") " unsigned 32bit division rounding to zero\n");
    else
        printf(GREEN("[PASS]") " unsigned 32bit division rounding to zero\n");
}
