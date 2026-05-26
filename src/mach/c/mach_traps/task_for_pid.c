#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t task_for_pid(mach_port_name_t target_tport, int pid, mach_port_name_t * t) {
    return mach_syscall(SYS_task_for_pid, target_tport, pid, (long)t);
}
