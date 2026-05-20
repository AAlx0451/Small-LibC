#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

#define mod(a, b) __modsi3(a, b)

void __modsi3_test()
{
    if (mod(100, 3) != 1)
        printf(RED("[FAIL]") " pos/pos 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/pos 32bit modulo\n");

    if (mod(-100, 3) != -1)
        printf(RED("[FAIL]") " neg/pos 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " neg/pos 32bit modulo\n");

    if (mod(100, -3) != 1)
        printf(RED("[FAIL]") " pos/neg 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/neg 32bit modulo\n");

    if (mod(-100, -3) != -1)
        printf(RED("[FAIL]") " neg/neg 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " neg/neg 32bit modulo\n");

    if (mod(100, 1) != 0)
        printf(RED("[FAIL]") " pos/1 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/1 32bit modulo\n");

    if (mod(100, -1) != 0)
        printf(RED("[FAIL]") " pos/-1 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/-1 32bit modulo\n");

    if (mod(100, 100) != 0)
        printf(RED("[FAIL]") " pos/self 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/self 32bit modulo\n");

    if (mod(INT_MAX, 1) != 0)
        printf(RED("[FAIL]") " INT_MAX/1 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " INT_MAX/1 32bit modulo\n");

    if (mod(INT_MAX, INT_MAX) != 0)
        printf(RED("[FAIL]") " INT_MAX/INT_MAX 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " INT_MAX/INT_MAX 32bit modulo\n");

    if (mod(INT_MAX, 2) != 1)
        printf(RED("[FAIL]") " INT_MAX/2 32bit modulo\n");
    else
        printf(GREEN("[PASS]") " INT_MAX/2 32bit modulo\n");
}
