#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strncmp_test()
{
    char abcde[] = "abcde";
    char abcdx[] = "abcdx";
    char cmpabcde[] = "abcde\0f";
    char cmpabcd_[] = "abcd\xfc";
    char empty[] = "";
    char x[] = "x";

    if (strncmp(abcde, cmpabcde, 5) != 0 || strncmp(abcde, cmpabcde, 10) != 0)
        printf(RED("[FAIL]") " strncmp matches identical prefixes and stops at null\n");
    else
        printf(GREEN("[PASS]") " strncmp matches identical prefixes and stops at null\n");

    if (strncmp(abcde, abcdx, 5) >= 0 || strncmp(empty, abcde, 5) >= 0)
        printf(RED("[FAIL]") " strncmp correctly signs inequality\n");
    else
        printf(GREEN("[PASS]") " strncmp correctly signs inequality\n");

    if (strncmp(abcde, abcdx, 4) != 0 || strncmp(abcde, x, 0) != 0)
        printf(RED("[FAIL]") " strncmp honors length limit n\n");
    else
        printf(GREEN("[PASS]") " strncmp honors length limit n\n");

    if (strncmp(abcde, x, 1) >= 0)
        printf(RED("[FAIL]") " strncmp compares single characters correctly\n");
    else
        printf(GREEN("[PASS]") " strncmp compares single characters correctly\n");

    if (strncmp(abcde, cmpabcd_, 10) >= 0)
        printf(RED("[FAIL]") " strncmp uses unsigned character comparison\n");
    else
        printf(GREEN("[PASS]") " strncmp uses unsigned character comparison\n");
}
