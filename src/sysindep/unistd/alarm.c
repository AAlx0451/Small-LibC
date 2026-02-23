#include <stddef.h> /* NULL */
#include <sys/time.h>

unsigned int alarm(unsigned int seconds) {
    struct itimerval new_it, old_it;
    new_it.it_interval.tv_sec = 0;
    new_it.it_interval.tv_usec = 0;
    new_it.it_value.tv_sec = (long)seconds;
    new_it.it_value.tv_usec = 0;
    if(setitimer(ITIMER_REAL, &new_it, &old_it) < 0) {
        return 0;
    }
    if(old_it.it_value.tv_usec > 0) {
        return (unsigned int)(old_it.it_value.tv_sec + 1);
    }
    return (unsigned int)old_it.it_value.tv_sec;
}
