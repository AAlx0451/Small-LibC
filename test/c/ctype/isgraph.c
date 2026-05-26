#include <ctype.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void isgraph_test()
{
    if (!isgraph('a') || !isgraph('z') || !isgraph('A') || !isgraph('Z') || !isgraph('@'))
        printf(RED("[FAIL]") " isgraph identifies graphic characters correctly\n");
    else
        printf(GREEN("[PASS]") " isgraph identifies graphic characters correctly\n");

    if (isgraph('\t') || isgraph('\0') || isgraph(' '))
        printf(RED("[FAIL]") " isgraph rejects non-graphic characters correctly\n");
    else
        printf(GREEN("[PASS]") " isgraph rejects non-graphic characters correctly\n");
}
