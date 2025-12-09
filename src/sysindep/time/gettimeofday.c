#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>

int gettimeofday(struct timeval *tv, void *tz)
{
    long ret = syscall(SYS_gettimeofday, (long)tv, (long)tz);
    if (ret < 0) {
        return -1;
    }
    return 0;
}
