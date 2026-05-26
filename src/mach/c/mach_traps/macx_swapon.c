#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t macx_swapon(uint64_t filename, int flags, int size, int priority) {
    return mach_syscall(SYS_macx_swapon, filename, flags, size, priority);
}
