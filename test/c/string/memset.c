#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void memset_test()
{
    char s[] = "xxxxxxxxx";

    if (memset(s, 'o', 10) != s || s[9] != 'o')
        printf(RED("[FAIL]") " memset fills memory and returns destination pointer\n");
    else
        printf(GREEN("[PASS]") " memset fills memory and returns destination pointer\n");

    if (memset(s, '_', (0)) != s || s[0] != 'o')
        printf(RED("[FAIL]") " memset does nothing when n is zero\n");
    else
        printf(GREEN("[PASS]") " memset does nothing when n is zero\n");

    if (memset(s, '_', 1) != s || s[0] != '_' || s[1] != 'o')
        printf(RED("[FAIL]") " memset fills exactly n bytes\n");
    else
        printf(GREEN("[PASS]") " memset fills exactly n bytes\n");
}
