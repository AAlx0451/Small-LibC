#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t macx_triggers(int hi_water, int low_water, int flags, mach_port_t alert_port) {
    return mach_syscall(SYS_macx_triggers, hi_water, low_water, flags, alert_port);
}
