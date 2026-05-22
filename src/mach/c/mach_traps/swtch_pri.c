#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

boolean_t swtch_pri(int pri) {
    return mach_syscall(SYS_swtch_pri, pri);
}
