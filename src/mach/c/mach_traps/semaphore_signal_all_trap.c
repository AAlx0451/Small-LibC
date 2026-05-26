#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t semaphore_signal_all_trap(mach_port_name_t signal_name)
{
    return mach_syscall(SYS_semaphore_signal_all_trap, signal_name);
}
