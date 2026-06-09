#include <mach/mach_traps.h>
#include <mach/semaphore.h>

kern_return_t semaphore_signal(semaphore_t semaphore) {
    return semaphore_signal_trap(semaphore);
}
