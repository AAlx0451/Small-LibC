#include <ctype.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void ispunct_test()
{
    if (!ispunct('@') || !ispunct('.'))
        printf(RED("[FAIL]") " ispunct identifies punctuation characters correctly\n");
    else
        printf(GREEN("[PASS]") " ispunct identifies punctuation characters correctly\n");

    if (ispunct('a') || ispunct('z') || ispunct('A') || ispunct('Z') || ispunct('\t') ||
        ispunct('\0') || ispunct(' '))
        printf(RED("[FAIL]") " ispunct rejects non-punctuation characters correctly\n");
    else
        printf(GREEN("[PASS]") " ispunct rejects non-punctuation characters correctly\n");
}
