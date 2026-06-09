#include <mach/mach_traps.h>
#include <mach/semaphore.h>

kern_return_t semaphore_signal_all(semaphore_t semaphore) {
    return semaphore_signal_all_trap(semaphore);
}
