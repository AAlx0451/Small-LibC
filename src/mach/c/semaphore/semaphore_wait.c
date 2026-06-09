#include <mach/mach_traps.h>
#include <mach/semaphore.h>

kern_return_t semaphore_wait(semaphore_t semaphore) {
    return semaphore_wait_trap(semaphore);
}
