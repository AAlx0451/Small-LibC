#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void gettimeofday_test()
{
    struct timeval tv;
    int tim = time(NULL);

    if (gettimeofday(&tv, NULL))
        printf(RED("[FAIL]") " gettimeofday returned non-zero\n");
    else
        printf(GREEN("[PASS]") " gettimeofday returned zero\n");

    if (tv.tv_usec >= 0 && tv.tv_usec < 1000000 && tv.tv_sec >= 0)
        printf(GREEN("[PASS]") " gettimeofday time sanity\n");
    else
        printf(RED("[FAIL]") " gettimeofday time sanity\n");

    if (tim == tv.tv_sec)
        printf(GREEN("[PASS]") " gettimeofday-time consistency\n");
    else
        printf(RED("[FAIL]") " gettimeofday-time consistency\n");
}
