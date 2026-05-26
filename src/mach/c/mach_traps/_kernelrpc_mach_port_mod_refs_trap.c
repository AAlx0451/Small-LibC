#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t _kernelrpc_mach_port_mod_refs_trap(mach_port_name_t target, mach_port_name_t name, mach_port_right_t right, mach_port_delta_t delta) {
    return mach_syscall(SYS_kernelrpc_mach_port_mod_refs_trap, target, name, right, delta);
}
