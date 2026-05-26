#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t _kernelrpc_mach_vm_allocate_trap(mach_port_name_t target, mach_vm_offset_t * addr, mach_vm_size_t size, int flags) {
    return mach_syscall(SYS_kernelrpc_mach_vm_allocate_trap, target, (long)addr, size, flags);
}
