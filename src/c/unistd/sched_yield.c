#include <sched.h>
#include <mach/mach_traps.h>

int sched_yield(void)
{
    return ((int)swtch_pri(0));
}
