#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t _kernelrpc_mach_port_allocate_trap(mach_port_name_t target, mach_port_right_t right, mach_port_name_t * name) {
    return mach_syscall(SYS_kernelrpc_mach_port_allocate_trap, target, right, (long)name);
}
