#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

#define div(a, b) __divdi3(a, b)

void __divdi3_test()
{
    if (div(68719476736, 2) != 34359738368)
        printf(RED("[FAIL]") " pos/pos 64bit division\n");
    else
        printf(GREEN("[PASS]") " pos/pos 64bit division\n");

    if (div(-68719476736, 2) != -34359738368)
        printf(RED("[FAIL]") " neg/pos 64bit division\n");
    else
        printf(GREEN("[PASS]") " neg/pos 64bit division\n");

    if (div(68719476736, -2) != -34359738368)
        printf(RED("[FAIL]") " pos/neg 64bit division\n");
    else
        printf(GREEN("[PASS]") " pos/neg 64bit division\n");

    if (div(-68719476736, -2) != 34359738368)
        printf(RED("[FAIL]") " neg/neg 64bit division\n");
    else
        printf(GREEN("[PASS]") " neg/neg 64bit division\n");

    if (div(68719476736, 1) != 68719476736)
        printf(RED("[FAIL]") " pos/1 64bit division\n");
    else
        printf(GREEN("[PASS]") " pos/1 64bit division\n");

    if (div(68719476736, -1) != -68719476736)
        printf(RED("[FAIL]") " pos/-1 64bit division\n");
    else
        printf(GREEN("[PASS]") " pos/-1 64bit division\n");

    if (div(LLONG_MAX, 1) != LLONG_MAX)
        printf(RED("[FAIL]") " LLONG_MAX/1 64bit division\n");
    else
        printf(GREEN("[PASS]") " LLONG_MAX/1 64bit division\n");

    if (div(LLONG_MAX, LLONG_MAX) != 1)
        printf(RED("[FAIL]") " LLONG_MAX/LLONG_MAX 64bit division\n");
    else
        printf(GREEN("[PASS]") " LLONG_MAX/LLONG_MAX 64bit division\n");

    if (div(7, 3) != 2 || div(-7, 3) != -2 || div(7, -3) != -2)
        printf(RED("[FAIL]") " 64bit division rounding to zero\n");
    else
        printf(GREEN("[PASS]") " 64bit division rounding to zero\n");

    if (div(1, 0) != 0)
        printf(RED("[FAIL]") " 1/0 == 0\n");
    else
        printf(GREEN("[PASS]") " 1/0 == 0\n");

    if (div(LLONG_MIN, -1) != LLONG_MIN)
        printf(RED("[FAIL]") " LLONG_MIN/-1 == LLONG_MIN\n");
    else
        printf(GREEN("[PASS]") " LLONG_MIN/-1 == LLONG_MIN\n");
}
