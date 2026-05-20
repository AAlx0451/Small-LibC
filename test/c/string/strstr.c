#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strstr_test()
{
    char s[] = "abcabcabcdabcde";

    if (strstr(s, "x") != NULL || strstr(s, "xyz") != NULL)
        printf(RED("[FAIL]") " strstr returns NULL when needle is not found\n");
    else
        printf(GREEN("[PASS]") " strstr returns NULL when needle is not found\n");

    if (strstr(s, "a") != &s[0] || strstr(s, "abc") != &s[0])
        printf(RED("[FAIL]") " strstr finds needle at the start of the string\n");
    else
        printf(GREEN("[PASS]") " strstr finds needle at the start of the string\n");

    if (strstr(s, "abcd") != &s[6] || strstr(s, "abcde") != &s[10])
        printf(RED("[FAIL]") " strstr handles partial matches and finds needle later in string\n");
    else
        printf(
            GREEN("[PASS]") " strstr handles partial matches and finds needle later in string\n");
}
