#include <mach/mach_traps.h> // mach_msg_overwrite_trap
#include <mach/message.h>

mach_msg_return_t mach_msg_overwrite(mach_msg_header_t *msg,
                                     mach_msg_option_t option,
                                     mach_msg_size_t send_size,
                                     mach_msg_size_t rcv_limit,
                                     mach_port_t rcv_name,
                                     mach_msg_timeout_t timeout,
                                     mach_port_t notify,
                                     mach_msg_header_t *rcv_msg,
                                     mach_msg_size_t rcv_scatter_size)
{
    mach_msg_return_t status;

    status = mach_msg_overwrite_trap(
        msg, option, send_size, rcv_limit, rcv_name, timeout, notify, rcv_msg, rcv_scatter_size);

    while (status == MACH_SEND_INTERRUPTED) {
        if (option & MACH_SEND_INTERRUPT) {
            return status;
        }

        status = mach_msg_overwrite_trap(msg,
                                         option,
                                         send_size,
                                         rcv_limit,
                                         rcv_name,
                                         timeout,
                                         notify,
                                         rcv_msg,
                                         rcv_scatter_size);
    }

    while (status == MACH_RCV_INTERRUPTED) {
        if (option & MACH_RCV_INTERRUPT) {
            return status;
        }

        option &= ~MACH_SEND_MSG;
        send_size = 0;

        status = mach_msg_overwrite_trap(msg,
                                         option,
                                         send_size,
                                         rcv_limit,
                                         rcv_name,
                                         timeout,
                                         notify,
                                         rcv_msg,
                                         rcv_scatter_size);
    }

    return status;
}
