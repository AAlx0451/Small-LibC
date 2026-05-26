#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

mach_port_name_t task_self_trap(void) { return mach_syscall(SYS_task_self_trap); }
