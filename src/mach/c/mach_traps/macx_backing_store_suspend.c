#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t macx_backing_store_suspend(boolean_t suspend) {
    return mach_syscall(SYS_macx_backing_store_suspend, suspend);
}
