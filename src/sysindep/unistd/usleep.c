#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

int usleep(unsigned long usec) {
    struct timeval tv;
    tv.tv_sec = usec / 1000000;
    tv.tv_usec = usec % 1000000;
    return select(0, NULL, NULL, NULL, &tv);
}
