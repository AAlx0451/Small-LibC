#include <mach/mach_syscalls.h>
#include <mach/mach_traps.h>

mach_msg_return_t mach_msg_overwrite_trap(mach_msg_header_t *msg,
                                          mach_msg_option_t option,
                                          mach_msg_size_t send_size,
                                          mach_msg_size_t rcv_size,
                                          mach_port_name_t rcv_name,
                                          mach_msg_timeout_t timeout,
                                          mach_port_name_t notify,
                                          mach_msg_header_t *rcv_msg,
                                          mach_msg_size_t rcv_limit)
{
    return mach_syscall(SYS_mach_msg_overwrite_trap,
                        (long)msg,
                        option,
                        send_size,
                        rcv_size,
                        rcv_name,
                        timeout,
                        notify,
                        (long)rcv_msg,
                        rcv_limit);
}
