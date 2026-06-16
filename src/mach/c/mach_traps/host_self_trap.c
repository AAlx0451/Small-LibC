#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

mach_port_name_t host_self_trap(void) { return (mach_port_name_t)mach_syscall(SYS_host_self_trap); }
