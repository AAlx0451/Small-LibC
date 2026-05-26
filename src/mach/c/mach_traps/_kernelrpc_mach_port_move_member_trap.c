#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t _kernelrpc_mach_port_move_member_trap(mach_port_name_t target,
                                                    mach_port_name_t member,
                                                    mach_port_name_t after)
{
    return mach_syscall(SYS_kernelrpc_mach_port_move_member_trap, target, member, after);
}
