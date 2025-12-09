#include <time.h>
#include <sys/time.h>

time_t time(time_t *tloc)
{
    struct timeval tv = {0}; 
    if (gettimeofday(&tv, NULL) < 0) {
        return (time_t)-1;
    }
    if (tloc != NULL) {
        *tloc = tv.tv_sec;
    }
    return tv.tv_sec;
}
