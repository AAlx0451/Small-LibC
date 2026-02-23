#include <sys/resource.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

clock_t clock(void) {
    struct rusage ru;
    if(getrusage(RUSAGE_SELF, &ru) != 0) {
        return (clock_t)-1;
    }

    long total_sec = ru.ru_utime.tv_sec + ru.ru_stime.tv_sec;
    long total_usec = ru.ru_utime.tv_usec + ru.ru_stime.tv_usec;
    clock_t result = (clock_t)(total_sec * CLOCKS_PER_SEC) +
                     (clock_t)(total_usec * (CLOCKS_PER_SEC / 1000000.0));

    return result;
}
