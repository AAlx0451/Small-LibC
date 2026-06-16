#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

mach_port_name_t mach_reply_port(void)
{
    return (mach_port_name_t)mach_syscall(SYS_mach_reply_port);
}
