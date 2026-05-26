#include <stdio.h>
#include <string.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strncat_test()
{
    char s[] = "xx\0xxxxxx";

    if (strncat(s, "abcde", 10) != s || s[2] != 'a' || s[6] != 'e' || s[7] != '\0' || s[8] != 'x')
        printf(RED("[FAIL]") " strncat(s, \"abcde\", 10) appends string correctly\n");
    else
        printf(GREEN("[PASS]") " strncat(s, \"abcde\", 10) appends string correctly\n");

    s[0] = '\0';
    if (strncat(s, "abcdx", 10) != s || s[4] != 'x' || s[5] != '\0')
        printf(RED("[FAIL]") " strncat(s, \"abcdx\", 10) on empty string works\n");
    else
        printf(GREEN("[PASS]") " strncat(s, \"abcdx\", 10) on empty string works\n");

    if (strncat(s, "", 10) != s || s[5] != '\0' || s[6] != 'e')
        printf(RED("[FAIL]") " strncat(s, \"\", 10) handles empty source\n");
    else
        printf(GREEN("[PASS]") " strncat(s, \"\", 10) handles empty source\n");

    if (strncat(s, "abcde", 0) != s || s[5] != '\0' || s[6] != 'e')
        printf(RED("[FAIL]") " strncat(s, \"abcde\", 0) handles zero count\n");
    else
        printf(GREEN("[PASS]") " strncat(s, \"abcde\", 0) handles zero count\n");

    if (strncat(s, "abcde", 3) != s || s[5] != 'a' || s[7] != 'c' || s[8] != '\0')
        printf(RED("[FAIL]") " strncat(s, \"abcde\", 3) truncates correctly\n");
    else
        printf(GREEN("[PASS]") " strncat(s, \"abcde\", 3) truncates correctly\n");
}
