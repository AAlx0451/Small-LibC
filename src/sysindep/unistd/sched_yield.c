#include <sched.h>

#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

int _sched_yield(void);

int sched_yield(void) {
    _sched_yield();
    return 0;
}
