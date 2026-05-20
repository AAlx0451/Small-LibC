#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strxfrm_test()
{
    char s[] = "xxxxxxxxxxx";

    if (strxfrm(NULL, "123456789012", 0) != 12)
        printf(RED("[FAIL]") " strxfrm returns correct length with n=0\n");
    else
        printf(GREEN("[PASS]") " strxfrm returns correct length with n=0\n");

    if (strxfrm(s, "1234567890", 11) != 10 || s[0] != '1' || s[9] != '0' || s[10] != '\0')
        printf(RED("[FAIL]") " strxfrm transforms string and null-terminates correctly\n");
    else
        printf(GREEN("[PASS]") " strxfrm transforms string and null-terminates correctly\n");
}
