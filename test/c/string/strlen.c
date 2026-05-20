#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strlen_test()
{
    if (strlen("abcde") != 5)
        printf(RED("[FAIL]") " strlen(\"abcde\") == 5\n");
    else
        printf(GREEN("[PASS]") " strlen(\"abcde\") == 5\n");

    if (strlen("") != 0)
        printf(RED("[FAIL]") " strlen(\"\") == 0\n");
    else
        printf(GREEN("[PASS]") " strlen(\"\") == 0\n");
}
