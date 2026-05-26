#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define GREEN(txt) "\033[0;32m" txt "\033[0m"
#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define unfdi(a)   __fixunsdfdi(a)
#define ULLNGMXFLT 18446744073709551615.0

void __fixunsdfdi_test()
{
    if (unfdi(0.0) != 0)
        printf(RED("[FAIL]") " (unsigned)0.0 == 0\n");
    else
        printf(GREEN("[PASS]") " (unsigned)0.0 == 0\n");

    if (unfdi(1.9) != 1)
        printf(RED("[FAIL]") " unsigned rounding to zero\n");
    else
        printf(GREEN("[PASS]") " unsigned rounding to zero\n");

    if (unfdi(ULLNGMXFLT) != ULLONG_MAX)
        printf(RED("[FAIL]") " (unsigned)(double)(ULLONG_MAX) == ULLONG_MAX\n");
    else
        printf(GREEN("[PASS]") " (unsigned)(double)(ULLONG_MAX) == ULLONG_MAX\n");

    if (unfdi(-1.0) != 0)
        printf(RED("[FAIL]") " negative float value to unsigned should be 0\n");
    else
        printf(GREEN("[PASS]") " negative float value to unsigned should be 0\n");
}
