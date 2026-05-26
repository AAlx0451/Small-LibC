#include <ctype.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void isdigit_test()
{
    if (!isdigit('0') || !isdigit('9'))
        printf(RED("[FAIL]") " isdigit identifies decimal digits correctly\n");
    else
        printf(GREEN("[PASS]") " isdigit identifies decimal digits correctly\n");

    if (isdigit(' ') || isdigit('a') || isdigit('@'))
        printf(RED("[FAIL]") " isdigit rejects non-digit characters correctly\n");
    else
        printf(GREEN("[PASS]") " isdigit rejects non-digit characters correctly\n");
}
