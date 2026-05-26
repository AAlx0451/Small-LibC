#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t _kernelrpc_mach_port_destroy_trap(mach_port_name_t target, mach_port_name_t name)
{
    return mach_syscall(SYS_kernelrpc_mach_port_destroy_trap, target, name);
}
