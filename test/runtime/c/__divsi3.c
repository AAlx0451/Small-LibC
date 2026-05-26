#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

#define div(a, b) __divsi3(a, b)

void __divsi3_test()
{
    if (div(100, 2) != 50)
        printf(RED("[FAIL]") " pos/pos 32bit division\n");
    else
        printf(GREEN("[PASS]") " pos/pos 32bit division\n");

    if (div(-100, 2) != -50)
        printf(RED("[FAIL]") " neg/pos 32bit division\n");
    else
        printf(GREEN("[PASS]") " neg/pos 32bit division\n");

    if (div(100, -2) != -50)
        printf(RED("[FAIL]") " pos/neg 32bit division\n");
    else
        printf(GREEN("[PASS]") " pos/neg 32bit division\n");

    if (div(-100, -2) != 50)
        printf(RED("[FAIL]") " neg/neg 32bit division\n");
    else
        printf(GREEN("[PASS]") " neg/neg 32bit division\n");

    if (div(100, 1) != 100)
        printf(RED("[FAIL]") " pos/1 32bit division\n");
    else
        printf(GREEN("[PASS]") " pos/1 32bit division\n");

    if (div(100, -1) != -100)
        printf(RED("[FAIL]") " pos/-1 32bit division\n");
    else
        printf(GREEN("[PASS]") " pos/-1 32bit division\n");

    if (div(INT_MAX, 1) != INT_MAX)
        printf(RED("[FAIL]") " INT_MAX/1 32bit division\n");
    else
        printf(GREEN("[PASS]") " INT_MAX/1 32bit division\n");

    if (div(INT_MAX, INT_MAX) != 1)
        printf(RED("[FAIL]") " INT_MAX/INT_MAX 32bit division\n");
    else
        printf(GREEN("[PASS]") " INT_MAX/INT_MAX 32bit division\n");

    if (div(7, 3) != 2 || div(-7, 3) != -2 || div(7, -3) != -2)
        printf(RED("[FAIL]") " 32bit division rounding to zero\n");
    else
        printf(GREEN("[PASS]") " 32bit division rounding to zero\n");
}
