#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strcat_test()
{
    char s[] = "xx\0xxxxxx";

    if (strcat(s, "abcde") != s)
        printf(RED("[FAIL]") " strcat returns destination pointer\n");
    else
        printf(GREEN("[PASS]") " strcat returns destination pointer\n");

    if (s[2] != 'a' || s[6] != 'e' || s[7] != '\0' || s[8] != 'x')
        printf(RED("[FAIL]") " strcat appends string and terminates correctly\n");
    else
        printf(GREEN("[PASS]") " strcat appends string and terminates correctly\n");

    s[0] = '\0';
    if (strcat(s, "abcdx") != s || s[4] != 'x' || s[5] != '\0')
        printf(RED("[FAIL]") " strcat appends to empty string correctly\n");
    else
        printf(GREEN("[PASS]") " strcat appends to empty string correctly\n");

    if (strcat(s, "") != s || s[5] != '\0' || s[6] != 'e')
        printf(RED("[FAIL]") " strcat handles appending empty string correctly\n");
    else
        printf(GREEN("[PASS]") " strcat handles appending empty string correctly\n");
}
