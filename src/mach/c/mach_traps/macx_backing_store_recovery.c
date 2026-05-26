#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t macx_backing_store_recovery(int pid) {
    return mach_syscall(SYS_macx_backing_store_recovery, pid);
}
