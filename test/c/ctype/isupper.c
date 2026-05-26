#include <ctype.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void isupper_test()
{
    if (!isupper('A') || !isupper('Z'))
        printf(RED("[FAIL]") " isupper identifies uppercase characters correctly\n");
    else
        printf(GREEN("[PASS]") " isupper identifies uppercase characters correctly\n");

    if (isupper('a') || isupper('z') || isupper(' ') || isupper('@'))
        printf(RED("[FAIL]") " isupper rejects non-uppercase characters correctly\n");
    else
        printf(GREEN("[PASS]") " isupper rejects non-uppercase characters correctly\n");
}
