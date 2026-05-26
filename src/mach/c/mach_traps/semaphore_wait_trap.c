#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t semaphore_wait_trap(mach_port_name_t wait_name)
{
    return mach_syscall(SYS_semaphore_wait_trap, wait_name);
}
