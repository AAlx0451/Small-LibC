#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t semaphore_timedwait_signal_trap(mach_port_name_t wait_name,
                                              mach_port_name_t signal_name,
                                              unsigned int sec,
                                              clock_res_t nsec)
{
    return mach_syscall(SYS_semaphore_timedwait_signal_trap, wait_name, signal_name, sec, nsec);
}
