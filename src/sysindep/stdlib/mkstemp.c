#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#define NUM_RETRIES 100
#define XX_LEN 6

int mkstemp(char *templat) {
    static int seeded = 0;
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";

    size_t len;
    char *xx;
    int fd;
    int i;
    int try_count;

    if(!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    if(templat == NULL) {
        errno = EINVAL;
        return -1;
    }

    len = strlen(templat);
    if(len < XX_LEN) {
        errno = EINVAL;
        return -1;
    }

    xx = templat + len - XX_LEN;
    for(i = 0; i < XX_LEN; ++i) {
        if(xx[i] != 'X') {
            errno = EINVAL;
            return -1;
        }
    }

    for(try_count = 0; try_count < NUM_RETRIES; ++try_count) {
        for(i = 0; i < XX_LEN; ++i) {
            xx[i] = charset[(unsigned)rand() % (sizeof(charset) - 1)];
        }

        fd = open(templat, O_RDWR | O_CREAT | O_EXCL, 0600);

        if(fd >= 0) {
            return fd;
        }

        if(errno != EEXIST) {
            return -1;
        }
    }

    errno = EEXIST;
    return -1;
}
