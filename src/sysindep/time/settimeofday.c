#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>

int settimeofday(const struct timeval *tv, const struct timezone *tz) {
    return syscall(SYS_settimeofday, (long)tv, (long)tz);
}
