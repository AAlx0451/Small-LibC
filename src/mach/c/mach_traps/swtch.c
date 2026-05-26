#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

boolean_t swtch(void) { return mach_syscall(SYS_swtch); }
