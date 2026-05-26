#include <ctype.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void toupper_test()
{
    if (toupper('a') != 'A' || toupper('z') != 'Z')
        printf(RED("[FAIL]") " toupper converts lowercase characters correctly\n");
    else
        printf(GREEN("[PASS]") " toupper converts lowercase characters correctly\n");

    if (toupper('A') != 'A' || toupper('Z') != 'Z' || toupper('@') != '@' || toupper('[') != '[')
        printf(RED("[FAIL]") " toupper leaves other characters unchanged\n");
    else
        printf(GREEN("[PASS]") " toupper leaves other characters unchanged\n");
}
