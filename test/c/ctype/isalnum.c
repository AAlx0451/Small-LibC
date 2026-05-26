#include <ctype.h>
#include <locale.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void isalnum_test()
{
    setlocale(LC_CTYPE, "C.UTF-8");
    if (!isalnum('a') || !isalnum('z') || !isalnum('A') || !isalnum('Z') || !isalnum('0') ||
        !isalnum('9'))
        printf(RED("[FAIL]") " isalnum identifies alphanumeric characters correctly\n");
    else
        printf(GREEN("[PASS]") " isalnum identifies alphanumeric characters correctly\n");

    if (isalnum(' ') || isalnum('\n') || isalnum('@'))
        printf(RED("[FAIL]") " isalnum rejects non-alphanumeric characters correctly\n");
    else
        printf(GREEN("[PASS]") " isalnum rejects non-alphanumeric characters correctly\n");
}
