#include <stdio.h>
#include <string.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strspn_test()
{
    char abcde[] = "abcde";

    if (strspn(abcde, "abc") != 3)
        printf(RED("[FAIL]") " strspn returns correct length for partial prefix match\n");
    else
        printf(GREEN("[PASS]") " strspn returns correct length for partial prefix match\n");

    if (strspn(abcde, "b") != 0)
        printf(RED("[FAIL]") " strspn returns 0 when first character is not in set\n");
    else
        printf(GREEN("[PASS]") " strspn returns 0 when first character is not in set\n");

    if (strspn(abcde, abcde) != 5)
        printf(RED("[FAIL]") " strspn returns full length when all characters match\n");
    else
        printf(GREEN("[PASS]") " strspn returns full length when all characters match\n");
}
