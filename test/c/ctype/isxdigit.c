#include <ctype.h>
#include <stdio.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void isxdigit_test()
{
    if (!isxdigit('0') || !isxdigit('9') || !isxdigit('a') || !isxdigit('f') || !isxdigit('A') ||
        !isxdigit('F'))
        printf(RED("[FAIL]") " isxdigit identifies hexadecimal digits correctly\n");
    else
        printf(GREEN("[PASS]") " isxdigit identifies hexadecimal digits correctly\n");

    if (isxdigit('g') || isxdigit('G') || isxdigit('@') || isxdigit(' '))
        printf(RED("[FAIL]") " isxdigit rejects non-hexadecimal characters correctly\n");
    else
        printf(GREEN("[PASS]") " isxdigit rejects non-hexadecimal characters correctly\n");
}
