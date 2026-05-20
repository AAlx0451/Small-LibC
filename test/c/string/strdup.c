#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strdup_test()
{
    const char *s = "abcde";
    char *d = strdup(s);

    if (d == NULL || d == s || strcmp(d, s) != 0)
        printf(RED("[FAIL]") " strdup creates a correct copy of the string\n");
    else
        printf(GREEN("[PASS]") " strdup creates a correct copy of the string\n");

    if (d != NULL) {
        d[0] = 'x';
        if (s[0] == 'x')
            printf(RED("[FAIL]") " strdup memory is not independent from source\n");
        else
            printf(GREEN("[PASS]") " strdup memory is independent from source\n");
        free(d);
    }

    char *empty_dup = strdup("");
    if (empty_dup == NULL || strcmp(empty_dup, "") != 0 || empty_dup[0] != '\0')
        printf(RED("[FAIL]") " strdup handles empty string correctly\n");
    else
        printf(GREEN("[PASS]") " strdup handles empty string correctly\n");
    free(empty_dup);
}
