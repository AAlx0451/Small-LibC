#include "runtime.h"
#include <limits.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

#define mod(a, b) __moddi3(a, b)

void __moddi3_test()
{
    if (mod(68719476739LL, 2LL) != 1LL)
        printf(RED("[FAIL]") " pos/pos 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/pos 64bit modulo\n");

    if (mod(-68719476739LL, 2LL) != -1LL)
        printf(RED("[FAIL]") " neg/pos 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " neg/pos 64bit modulo\n");

    if (mod(68719476739LL, -2LL) != 1LL)
        printf(RED("[FAIL]") " pos/neg 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/neg 64bit modulo\n");

    if (mod(-68719476739LL, -2LL) != -1LL)
        printf(RED("[FAIL]") " neg/neg 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " neg/neg 64bit modulo\n");

    if (mod(68719476739LL, 1LL) != 0LL)
        printf(RED("[FAIL]") " pos/1 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/1 64bit modulo\n");

    if (mod(68719476739LL, -1LL) != 0LL)
        printf(RED("[FAIL]") " pos/-1 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/-1 64bit modulo\n");

    if (mod(68719476739LL, 68719476739LL) != 0LL)
        printf(RED("[FAIL]") " pos/self 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " pos/self 64bit modulo\n");

    if (mod(LLONG_MAX, 1LL) != 0LL)
        printf(RED("[FAIL]") " LLONG_MAX/1 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " LLONG_MAX/1 64bit modulo\n");

    if (mod(LLONG_MAX, LLONG_MAX) != 0LL)
        printf(RED("[FAIL]") " LLONG_MAX/LLONG_MAX 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " LLONG_MAX/LLONG_MAX 64bit modulo\n");

    if (mod(LLONG_MAX, 2LL) != 1LL)
        printf(RED("[FAIL]") " LLONG_MAX/2 64bit modulo\n");
    else
        printf(GREEN("[PASS]") " LLONG_MAX/2 64bit modulo\n");
}
