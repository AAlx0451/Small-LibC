#include "runtime.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>

#define GREEN(txt) "\033[0;32m" txt "\033[0m"
#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define dfdi(a)    __fixdfdi(a)
#define LLNGMXFLT  9223372036854775807.0
#define LLNGMNFLT  -9223372036854775808.0

void __fixdfdi_test()
{
    if (dfdi(0.0) != 0)
        printf(RED("[FAIL]") " (int)0.0 == 0\n");
    else
        printf(GREEN("[PASS]") " (int)0.0 == 0\n");

    if (dfdi(-1.0) != -1)
        printf(RED("[FAIL]") " (int)-1.0 == -1\n");
    else
        printf(GREEN("[PASS]") " (int)-1.0 == -1\n");

    if (dfdi(-1.9) != -1)
        printf(RED("[FAIL]") " negative rounding to zero\n");
    else
        printf(GREEN("[PASS]") " negative rounding to zero\n");

    if (dfdi(1.9) != 1)
        printf(RED("[FAIL]") " positive rounding to zero\n");
    else
        printf(GREEN("[PASS]") " positive rounding to zero\n");

    if (dfdi(LLNGMXFLT) != LLONG_MAX)
        printf(RED("[FAIL]") " (int)(double)(LLONG_MAX) == LLONG_MAX\n");
    else
        printf(GREEN("[PASS]") " (int)(double)(LLONG_MAX) == LLONG_MAX\n");

    if (dfdi(LLNGMNFLT) != LLONG_MIN)
        printf(RED("[FAIL]") " (int)(double)(LLONG_MIN) == LLONG_MIN\n");
    else
        printf(GREEN("[PASS]") " (int)(double)(LLONG_MIN) == LLONG_MIN\n");

    if (dfdi(NAN) != 0)
        printf(RED("[FAIL]") " (int)NAN == 0\n");
    else
        printf(GREEN("[PASS]") " (int)NAN == 0\n");
}
