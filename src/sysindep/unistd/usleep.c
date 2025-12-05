#include <sys/select.h>
#include <sys/time.h>

void usleep(long usec) {
    struct timeval tv;
    tv.tv_sec = usec / 1000000;
    tv.tv_usec = usec % 1000000;
    select(0, NULL, NULL, NULL, &tv);
}
