#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void memchr_test()
{
    char abcde[] = "abcde";

    if (memchr(abcde, 'c', 5) != &abcde[2] || memchr(abcde, 'a', 1) != &abcde[0])
        printf(RED("[FAIL]") " memchr finds characters within the specified range\n");
    else
        printf(GREEN("[PASS]") " memchr finds characters within the specified range\n");

    if (memchr(abcde, 'a', 0) != NULL || memchr(abcde, '\0', 5) != NULL)
        printf(RED("[FAIL]") " memchr returns NULL if character is not found within n bytes\n");
    else
        printf(GREEN("[PASS]") " memchr returns NULL if character is not found within n bytes\n");

    if (memchr(abcde, '\0', 6) != &abcde[5])
        printf(RED("[FAIL]") " memchr finds the null terminator when n includes it\n");
    else
        printf(GREEN("[PASS]") " memchr finds the null terminator when n includes it\n");
}
