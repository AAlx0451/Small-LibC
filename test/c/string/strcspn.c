#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strcspn_test()
{
    char abcde[] = "abcde";
    char abcdx[] = "abcdx";

    if (strcspn(abcde, "xyz") != 5 || strcspn(abcde, "x") != 5)
        printf(RED("[FAIL]") " strcspn returns full length when no characters match\n");
    else
        printf(GREEN("[PASS]") " strcspn returns full length when no characters match\n");

    if (strcspn(abcdx, "xyz") != 4 || strcspn(abcdx, "x") != 4)
        printf(RED("[FAIL]") " strcspn returns index of first matching character\n");
    else
        printf(GREEN("[PASS]") " strcspn returns index of first matching character\n");

    if (strcspn(abcde, "abc") != 0 || strcspn(abcde, "cba") != 0)
        printf(RED("[FAIL]") " strcspn returns 0 when match is at the start\n");
    else
        printf(GREEN("[PASS]") " strcspn returns 0 when match is at the start\n");
}
