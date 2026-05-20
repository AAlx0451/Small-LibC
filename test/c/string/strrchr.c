#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strrchr_test()
{
    char abcde[] = "abcde";
    char abccd[] = "abccd";

    if (strrchr(abcde, '\0') != &abcde[5])
        printf(RED("[FAIL]") " strrchr finds the null terminator\n");
    else
        printf(GREEN("[PASS]") " strrchr finds the null terminator\n");

    if (strrchr(abcde, 'a') != &abcde[0] || strrchr(abcde, 'e') != &abcde[4])
        printf(RED("[FAIL]") " strrchr finds unique characters at boundaries\n");
    else
        printf(GREEN("[PASS]") " strrchr finds unique characters at boundaries\n");

    if (strrchr(abccd, 'c') != &abccd[3])
        printf(RED("[FAIL]") " strrchr finds the last occurrence of a repeated character\n");
    else
        printf(GREEN("[PASS]") " strrchr finds the last occurrence of a repeated character\n");
}
