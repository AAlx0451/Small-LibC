#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t clock_sleep_trap(mach_port_name_t clock_name,
                               sleep_type_t sleep_type,
                               int sleep_sec,
                               int sleep_nsec,
                               mach_timespec_t *wakeup_time)
{
    return mach_syscall(
        SYS_clock_sleep_trap, clock_name, sleep_type, sleep_sec, sleep_nsec, (long)wakeup_time);
}
