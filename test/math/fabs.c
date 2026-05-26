#include <float.h>
#include <math.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void fabs_test()
{
    if (fabs(5.5) != 5.5)
        printf(RED("[FAIL]") " fabs(5.5) == 5.5\n");
    else
        printf(GREEN("[PASS]") " fabs(5.5) == 5.5\n");

    if (fabs(-1.5) != 1.5)
        printf(RED("[FAIL]") " fabs(-1.5) == 1.5\n");
    else
        printf(GREEN("[PASS]") " fabs(-1.5) == 1.5\n");

    if (fabs(-INFINITY) != INFINITY)
        printf(RED("[FAIL]") " fabs(-INFINITY) == INFINITY\n");
    else
        printf(GREEN("[PASS]") " fabs(-INFINITY) == INFINITY\n");

    if (fabs(-DBL_MAX) != DBL_MAX)
        printf(RED("[FAIL]") " fabs(-DBL_MAX) == DBL_MAX\n");
    else
        printf(GREEN("[PASS]") " fabs(-DBL_MAX) == DBL_MAX\n");
}
