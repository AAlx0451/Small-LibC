#include <stdio.h>
#include <string.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strpbrk_test()
{
    char abcde[] = "abcde";
    char abcdx[] = "abcdx";

    if (strpbrk(abcde, "x") != NULL || strpbrk(abcde, "xyz") != NULL)
        printf(RED("[FAIL]") " strpbrk returns NULL when no characters match\n");
    else
        printf(GREEN("[PASS]") " strpbrk returns NULL when no characters match\n");

    if (strpbrk(abcdx, "zyx") != &abcdx[4] || strpbrk(abcdx, "x") != &abcdx[4])
        printf(RED("[FAIL]") " strpbrk finds matching character at the end of string\n");
    else
        printf(GREEN("[PASS]") " strpbrk finds matching character at the end of string\n");

    if (strpbrk(abcde, "cba") != &abcde[0] || strpbrk(abcde, "a") != &abcde[0])
        printf(RED("[FAIL]") " strpbrk finds matching character at the start of string\n");
    else
        printf(GREEN("[PASS]") " strpbrk finds matching character at the start of string\n");
}
