#include <ctype.h>
#include <stdio.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void isblank_test()
{
    if (!isblank(' ') || !isblank('\t'))
        printf(RED("[FAIL]") " isblank identifies blank characters correctly\n");
    else
        printf(GREEN("[PASS]") " isblank identifies blank characters correctly\n");

    if (isblank('\v') || isblank('\r') || isblank('x') || isblank('@'))
        printf(RED("[FAIL]") " isblank rejects non-blank characters correctly\n");
    else
        printf(GREEN("[PASS]") " isblank rejects non-blank characters correctly\n");
}
