#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void memcmp_test()
{
    char abcde[] = "abcde";
    char abcdx[] = "abcdx";
    char xxxxx[] = "xxxxx";

    if (memcmp(abcde, abcdx, 5) >= 0 || memcmp(xxxxx, abcde, 1) <= 0)
        printf(RED("[FAIL]") " memcmp correctly signs inequality\n");
    else
        printf(GREEN("[PASS]") " memcmp correctly signs inequality\n");

    if (memcmp(abcde, abcdx, 4) != 0)
        printf(RED("[FAIL]") " memcmp returns 0 when prefixes match within n bytes\n");
    else
        printf(GREEN("[PASS]") " memcmp returns 0 when prefixes match within n bytes\n");

    if (memcmp(abcde, xxxxx, 0) != 0)
        printf(RED("[FAIL]") " memcmp returns 0 when n is zero\n");
    else
        printf(GREEN("[PASS]") " memcmp returns 0 when n is zero\n");
}
