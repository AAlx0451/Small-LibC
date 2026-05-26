#include <ctype.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void tolower_test()
{
    if (tolower('A') != 'a' || tolower('Z') != 'z')
        printf(RED("[FAIL]") " tolower converts uppercase characters correctly\n");
    else
        printf(GREEN("[PASS]") " tolower converts uppercase characters correctly\n");

    if (tolower('a') != 'a' || tolower('z') != 'z' || tolower('@') != '@' || tolower('[') != '[')
        printf(RED("[FAIL]") " tolower leaves other characters unchanged\n");
    else
        printf(GREEN("[PASS]") " tolower leaves other characters unchanged\n");
}
