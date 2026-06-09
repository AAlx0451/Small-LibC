#include <mach/mach_traps.h>
#include <mach/semaphore.h>

kern_return_t semaphore_wait_signal(semaphore_t wait_semaphore, semaphore_t signal_semaphore) {
    return semaphore_wait_signal_trap(wait_semaphore, signal_semaphore);
}
