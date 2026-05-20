#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strcmp_test()
{
    char abcde[] = "abcde";
    char abcdx[] = "abcdx";
    char cmpabcde[] = "abcde";
    char cmpabcd_[] = "abcd\xfc";
    char empty[] = "";

    if (strcmp(abcde, cmpabcde) != 0)
        printf(RED("[FAIL]") " strcmp returns 0 for identical strings\n");
    else
        printf(GREEN("[PASS]") " strcmp returns 0 for identical strings\n");

    if (strcmp(abcde, abcdx) >= 0 || strcmp(abcdx, abcde) <= 0)
        printf(RED("[FAIL]") " strcmp correctly signs inequality\n");
    else
        printf(GREEN("[PASS]") " strcmp correctly signs inequality\n");

    if (strcmp(empty, abcde) >= 0 || strcmp(abcde, empty) <= 0)
        printf(RED("[FAIL]") " strcmp handles empty strings correctly\n");
    else
        printf(GREEN("[PASS]") " strcmp handles empty strings correctly\n");

    if (strcmp(abcde, cmpabcd_) >= 0)
        printf(RED("[FAIL]") " strcmp uses unsigned character comparison\n");
    else
        printf(GREEN("[PASS]") " strcmp uses unsigned character comparison\n");
}
