#include <ctype.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void isspace_test()
{
    if (!isspace(' ') || !isspace('\f') || !isspace('\n') || !isspace('\r') || !isspace('\t') ||
        !isspace('\v'))
        printf(RED("[FAIL]") " isspace identifies whitespace characters correctly\n");
    else
        printf(GREEN("[PASS]") " isspace identifies whitespace characters correctly\n");

    if (isspace('a'))
        printf(RED("[FAIL]") " isspace rejects non-whitespace characters correctly\n");
    else
        printf(GREEN("[PASS]") " isspace rejects non-whitespace characters correctly\n");
}
