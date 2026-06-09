#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t semaphore_timedwait_trap(mach_port_name_t wait_name,
                                       unsigned int sec,
                                       clock_res_t nsec)
{
    return mach_syscall(SYS_semaphore_timedwait_trap, wait_name, sec, nsec);
}
