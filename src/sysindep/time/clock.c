#include <sys/resource.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

clock_t clock(void) {
    long total_sec, total_usec;
    clock_t result;
    struct rusage ru;
    if(getrusage(RUSAGE_SELF, &ru) != 0) {
        return (clock_t)-1;
    }

    total_sec = ru.ru_utime.tv_sec + ru.ru_stime.tv_sec;
    total_usec = ru.ru_utime.tv_usec + ru.ru_stime.tv_usec;
    result = (clock_t)(total_sec * CLOCKS_PER_SEC) +
             (clock_t)((double)total_usec * (CLOCKS_PER_SEC / 1000000.0));

    return result;
}
