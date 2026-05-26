#include <stdio.h>
#include <stdlib.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void abs_test()
{
    if (abs(256) != 256)
        printf(RED("[FAIL]") " abs(256) == 256\n");
    else
        printf(GREEN("[PASS]") " abs(256) == 256\n");

    if (abs(-256) != 256)
        printf(RED("[FAIL]") " abs(-256) == 256\n");
    else
        printf(GREEN("[PASS]") " abs(-256) == 256\n");
}
