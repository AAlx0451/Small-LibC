#include <mach/mach_traps.h>
#include <sched.h>

int sched_yield(void) { return ((int)swtch_pri(0)); }
