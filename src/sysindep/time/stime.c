#include <errno.h>
#include <sys/time.h>
#include <time.h>

int stime(const time_t *t) {
    struct timeval tv;
    if(!t) {
        errno = EFAULT;
        return -1;
    }
    tv.tv_sec = *t;
    tv.tv_usec = 0;
    return settimeofday(&tv, NULL);
}
