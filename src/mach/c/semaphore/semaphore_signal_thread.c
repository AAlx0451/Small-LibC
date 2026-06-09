#include <mach/mach_traps.h>
#include <mach/semaphore.h>

kern_return_t semaphore_signal_thread(semaphore_t semaphore, thread_t thread)
{
    return semaphore_signal_thread_trap(semaphore, thread);
}
