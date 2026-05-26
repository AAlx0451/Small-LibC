#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t macx_swapoff(uint64_t filename, int flags)
{
    return mach_syscall(SYS_macx_swapoff, filename, flags);
}
