#include <stdio.h>
#include <string.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strchr_test()
{
    char abccd[] = "abccd";

    if (strchr(abccd, 'x') != NULL)
        printf(RED("[FAIL]") " strchr returns NULL when character is not found\n");
    else
        printf(GREEN("[PASS]") " strchr returns NULL when character is not found\n");

    if (strchr(abccd, 'a') != &abccd[0] || strchr(abccd, 'd') != &abccd[4])
        printf(RED("[FAIL]") " strchr finds characters at the start and end of string\n");
    else
        printf(GREEN("[PASS]") " strchr finds characters at the start and end of string\n");

    if (strchr(abccd, '\0') != &abccd[5])
        printf(RED("[FAIL]") " strchr finds the null terminator\n");
    else
        printf(GREEN("[PASS]") " strchr finds the null terminator\n");

    if (strchr(abccd, 'c') != &abccd[2])
        printf(RED("[FAIL]") " strchr finds the first occurrence of a repeated character\n");
    else
        printf(GREEN("[PASS]") " strchr finds the first occurrence of a repeated character\n");
}
