#include <errno.h>
#include <stdio.h>
#include <string.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strerror_test()
{
    char range_msg[256];
    char dom_msg[256];

    strncpy(range_msg, strerror(ERANGE), sizeof(range_msg) - 1);
    range_msg[sizeof(range_msg) - 1] = '\0';

    strncpy(dom_msg, strerror(EDOM), sizeof(dom_msg) - 1);
    dom_msg[sizeof(dom_msg) - 1] = '\0';

    if (strcmp(range_msg, dom_msg) == 0)
        printf(RED("[FAIL]") " strerror messages for ERANGE and EDOM are identical\n");
    else
        printf(GREEN("[PASS]") " strerror messages for ERANGE and EDOM are different\n");
}
