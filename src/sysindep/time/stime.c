#include <errno.h>
#include <sys/time.h>

int stime(const time_t *t) {
    if (!t) {
        errno = EFAULT;
        return -1;
    }
    struct timeval tv;
    tv.tv_sec = *t;
    tv.tv_usec = 0;
    return settimeofday(&tv, NULL);
}
