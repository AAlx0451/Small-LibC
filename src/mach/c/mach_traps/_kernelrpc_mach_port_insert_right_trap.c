#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t _kernelrpc_mach_port_insert_right_trap(mach_port_name_t target,
                                                     mach_port_name_t name,
                                                     mach_port_name_t poly,
                                                     mach_msg_type_name_t polyPoly)
{
    return mach_syscall(SYS_kernelrpc_mach_port_insert_right_trap, target, name, poly, polyPoly);
}
