#include <stdio.h>
#include <string.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strcoll_test()
{
    char abcde[] = "abcde";
    char abcdx[] = "abcdx";
    char empty[] = "";

    if (strcoll(abcde, "abcde") != 0)
        printf(RED("[FAIL]") " strcoll returns 0 for identical strings\n");
    else
        printf(GREEN("[PASS]") " strcoll returns 0 for identical strings\n");

    if (strcoll(abcde, abcdx) >= 0 || strcoll(abcdx, abcde) <= 0)
        printf(RED("[FAIL]") " strcoll correctly compares non-identical strings\n");
    else
        printf(GREEN("[PASS]") " strcoll correctly compares non-identical strings\n");

    if (strcoll(empty, abcde) >= 0 || strcoll(abcde, empty) <= 0)
        printf(RED("[FAIL]") " strcoll handles empty strings correctly\n");
    else
        printf(GREEN("[PASS]") " strcoll handles empty strings correctly\n");
}
