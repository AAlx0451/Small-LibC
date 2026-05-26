#include <stdio.h>
#include <string.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void memmove_test()
{
    char s[] = "xxxxabcde";

    if (memmove(s, s + 4, 5) != s || s[0] != 'a' || s[4] != 'e' || s[5] != 'b')
        printf(RED("[FAIL]") " memmove handles overlap copying to the left correctly\n");
    else
        printf(GREEN("[PASS]") " memmove handles overlap copying to the left correctly\n");

    if (memmove(s + 4, s, 5) != s + 4 || s[4] != 'a')
        printf(RED("[FAIL]") " memmove handles overlap copying to the right correctly\n");
    else
        printf(GREEN("[PASS]") " memmove handles overlap copying to the right correctly\n");
}
