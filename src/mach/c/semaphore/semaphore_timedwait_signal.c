#include <mach/mach_traps.h>
#include <mach/semaphore.h>

kern_return_t semaphore_timedwait_signal(semaphore_t wait_semaphore, semaphore_t signal_semaphore, mach_timespec_t wait_time) {
    return semaphore_timedwait_signal_trap(wait_semaphore, signal_semaphore, wait_time.tv_sec, wait_time.tv_nsec);
}
