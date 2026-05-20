#include <ctype.h>
#include <stdio.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void iscntrl_test()
{
    if (!iscntrl('\a') || !iscntrl('\b') || !iscntrl('\n'))
        printf(RED("[FAIL]") " iscntrl identifies control characters correctly\n");
    else
        printf(GREEN("[PASS]") " iscntrl identifies control characters correctly\n");

    if (iscntrl(' '))
        printf(RED("[FAIL]") " iscntrl rejects printable characters correctly\n");
    else
        printf(GREEN("[PASS]") " iscntrl rejects printable characters correctly\n");
}
