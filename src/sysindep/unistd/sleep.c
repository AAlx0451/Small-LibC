#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

int sleep(unsigned int seconds) {
    struct timeval tv;
    tv.tv_sec = (time_t)seconds;
    tv.tv_usec = 0;
    return select(0, 0, 0, 0, &tv);
}
