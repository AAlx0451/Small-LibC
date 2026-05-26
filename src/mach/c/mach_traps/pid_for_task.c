#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t pid_for_task(mach_port_name_t t, int * x) {
    return mach_syscall(SYS_pid_for_task, t, (long)x);
}
