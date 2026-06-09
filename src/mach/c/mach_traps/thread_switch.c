#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

kern_return_t thread_switch(mach_port_name_t thread_name,
                            int option,
                            mach_msg_timeout_t option_time)
{
    return mach_syscall(SYS_thread_switch, thread_name, option, option_time);
}
