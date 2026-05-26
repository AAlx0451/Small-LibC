#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t task_name_for_pid(mach_port_name_t target_tport, int pid, mach_port_name_t * tn) {
    return mach_syscall(SYS_task_name_for_pid, target_tport, pid, (long)tn);
}
