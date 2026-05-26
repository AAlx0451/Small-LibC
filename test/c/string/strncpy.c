#include <stdio.h>
#include <string.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strncpy_test()
{
    char s[] = "xxxxxxx";

    if (strncpy(s, "", 1) != s || s[0] != '\0' || s[1] != 'x')
        printf(RED("[FAIL]") " strncpy(s, \"\", 1) copies null and preserves tail\n");
    else
        printf(GREEN("[PASS]") " strncpy(s, \"\", 1) copies null and preserves tail\n");

    if (strncpy(s, "abcde", 6) != s || s[4] != 'e' || s[5] != '\0' || s[6] != 'x')
        printf(RED("[FAIL]") " strncpy(s, \"abcde\", 6) null-terminates and preserves tail\n");
    else
        printf(GREEN("[PASS]") " strncpy(s, \"abcde\", 6) null-terminates and preserves tail\n");

    if (strncpy(s, "abcde", 7) != s || s[6] != '\0')
        printf(RED("[FAIL]") " strncpy(s, \"abcde\", 7) pads remaining bytes with null\n");
    else
        printf(GREEN("[PASS]") " strncpy(s, \"abcde\", 7) pads remaining bytes with null\n");

    if (strncpy(s, "xxxx", 3) != s || s[2] != 'x' || s[3] != 'd')
        printf(RED("[FAIL]") " strncpy(s, \"xxxx\", 3) does not null-terminate if n < length\n");
    else
        printf(GREEN("[PASS]") " strncpy(s, \"xxxx\", 3) does not null-terminate if n < length\n");
}
