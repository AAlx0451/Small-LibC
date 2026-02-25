#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#pragma clang diagnostic ignored "-Wreserved-identifier"

static inline clock_t _timeval_to_clock_t(const struct timeval *tv) {
    long ticks_per_sec = CLK_TCK;
    return (clock_t)((tv->tv_sec * ticks_per_sec) +
                     (tv->tv_usec * ticks_per_sec) / 1000000);
}

clock_t times(struct tms *buf) {
    struct rusage ru_self;
    struct rusage ru_child;
    struct timeval now;

    if(getrusage(RUSAGE_SELF, &ru_self) != 0) {
        return (clock_t)-1;
    }

    if(getrusage(RUSAGE_CHILDREN, &ru_child) != 0) {
        return (clock_t)-1;
    }

    if(buf) {
        buf->tms_utime = _timeval_to_clock_t(&ru_self.ru_utime);
        buf->tms_stime = _timeval_to_clock_t(&ru_self.ru_stime);
        buf->tms_cutime = _timeval_to_clock_t(&ru_child.ru_utime);
        buf->tms_cstime = _timeval_to_clock_t(&ru_child.ru_stime);
    }

    if(gettimeofday(&now, (void *)0) != 0) {
        return (clock_t)-1;
    }

    return _timeval_to_clock_t(&now);
}
