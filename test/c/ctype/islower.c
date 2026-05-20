#include <ctype.h>
#include <stdio.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void islower_test()
{
    if (!islower('a') || !islower('z'))
        printf(RED("[FAIL]") " islower identifies lowercase characters correctly\n");
    else
        printf(GREEN("[PASS]") " islower identifies lowercase characters correctly\n");

    if (islower('A') || islower('Z') || islower(' ') || islower('@'))
        printf(RED("[FAIL]") " islower rejects non-lowercase characters correctly\n");
    else
        printf(GREEN("[PASS]") " islower rejects non-lowercase characters correctly\n");
}
