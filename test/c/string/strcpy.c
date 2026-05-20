#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strcpy_test()
{
    char s[] = "xxxxx";

    if (strcpy(s, "") != s || s[0] != '\0' || s[1] != 'x')
        printf(RED("[FAIL]") " strcpy(s, \"\") handles empty string correctly\n");
    else
        printf(GREEN("[PASS]") " strcpy(s, \"\") handles empty string correctly\n");

    if (strcpy(s, "abcde") != s || s[0] != 'a' || s[4] != 'e' || s[5] != '\0')
        printf(RED("[FAIL]") " strcpy(s, \"abcde\") copies content and null terminator\n");
    else
        printf(GREEN("[PASS]") " strcpy(s, \"abcde\") copies content and null terminator\n");
}
