#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void memcpy_test()
{
    char s[] = "xxxxxxxxxxx";
    char abcde[] = "abcde";

    if (memcpy(s, abcde, 6) != s || s[4] != 'e' || s[5] != '\0')
        printf(RED("[FAIL]") " memcpy copies data and returns destination pointer\n");
    else
        printf(GREEN("[PASS]") " memcpy copies data and returns destination pointer\n");

    if (memcpy(s + 5, abcde, 5) != s + 5 || s[9] != 'e' || s[10] != 'x')
        printf(RED("[FAIL]") " memcpy copies to offset and respects length n\n");
    else
        printf(GREEN("[PASS]") " memcpy copies to offset and respects length n\n");
}
