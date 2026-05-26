#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t _kernelrpc_mach_vm_deallocate_trap(mach_port_name_t target,
                                                 mach_vm_address_t address,
                                                 mach_vm_size_t size)
{
    return mach_syscall(SYS_kernelrpc_mach_vm_deallocate_trap, target, address, size);
}
