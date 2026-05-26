#include <ctype.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void isalpha_test()
{
    if (!isalpha('a') || !isalpha('z'))
        printf(RED("[FAIL]") " isalpha identifies alphabetic characters correctly\n");
    else
        printf(GREEN("[PASS]") " isalpha identifies alphabetic characters correctly\n");

    if (isalpha(' ') || isalpha('1') || isalpha('@'))
        printf(RED("[FAIL]") " isalpha rejects non-alphabetic characters correctly\n");
    else
        printf(GREEN("[PASS]") " isalpha rejects non-alphabetic characters correctly\n");
}
