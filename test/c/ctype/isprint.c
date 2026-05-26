#include <ctype.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void isprint_test()
{
    if (!isprint('a') || !isprint('z') || !isprint('A') || !isprint('Z') || !isprint('@') ||
        !isprint(' '))
        printf(RED("[FAIL]") " isprint identifies printable characters correctly\n");
    else
        printf(GREEN("[PASS]") " isprint identifies printable characters correctly\n");

    if (isprint('\t') || isprint('\0'))
        printf(RED("[FAIL]") " isprint rejects non-printable characters correctly\n");
    else
        printf(GREEN("[PASS]") " isprint rejects non-printable characters correctly\n");
}
