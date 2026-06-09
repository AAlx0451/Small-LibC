#include <mach/mach_traps.h>
#include <mach/semaphore.h>

kern_return_t semaphore_timedwait(semaphore_t semaphore, mach_timespec_t wait_time)
{
    return semaphore_timedwait_trap(semaphore, wait_time.tv_sec, wait_time.tv_nsec);
}
