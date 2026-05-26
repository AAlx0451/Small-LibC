#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

#define umod(a, b) __umodsi3(a, b)

void __umodsi3_test()
{
    if (umod(100U, 3U) != 1U)
        printf(RED("[FAIL]") " pos/pos unsigned 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/pos unsigned 32bit modulo\n");

    if (umod(100U, 1U) != 0U)
        printf(RED("[FAIL]") " pos/1 unsigned 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/1 unsigned 32bit modulo\n");

    if (umod(100U, 100U) != 0U)
        printf(RED("[FAIL]") " pos/self unsigned 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/self unsigned 32bit modulo\n");

    if (umod(UINT_MAX, 1U) != 0U)
        printf(RED("[FAIL]") " UINT_MAX/1 unsigned 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " UINT_MAX/1 unsigned 32bit modulo\n");

    if (umod(UINT_MAX, UINT_MAX) != 0U)
        printf(RED("[FAIL]") " UINT_MAX/UINT_MAX unsigned 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " UINT_MAX/UINT_MAX unsigned 32bit modulo\n");

    if (umod(UINT_MAX, 2U) != 1U)
        printf(RED("[FAIL]") " UINT_MAX/2 unsigned 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " UINT_MAX/2 unsigned 32bit modulo\n");

    if (umod(UINT_MAX, 3U) != (UINT_MAX % 3U))
        printf(RED("[FAIL]") " UINT_MAX/3 unsigned 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " UINT_MAX/3 unsigned 32bit modulo\n");
}
