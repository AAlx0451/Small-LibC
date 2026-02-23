#include <sys/time.h>
#include <utime.h>

int utime(const char *path, const struct utimbuf *times) {
    if(times) {
        struct timeval tv[2];

        tv[0].tv_sec = times->actime;
        tv[0].tv_usec = 0;

        tv[1].tv_sec = times->modtime;
        tv[1].tv_usec = 0;

        return utimes(path, tv);
    }

    return utimes(path, NULL);
}
