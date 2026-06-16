#include <mach/mach_traps.h>
#include <mach/message.h>
#include <mach/port.h>

mach_msg_return_t mach_msg(mach_msg_header_t *msg,
                           mach_msg_option_t option,
                           mach_msg_size_t send_size,
                           mach_msg_size_t rcv_size,
                           mach_port_t rcv_name,
                           mach_msg_timeout_t timeout,
                           mach_port_t notify)
{
    mach_msg_option_t rcv_only_option;
    mach_msg_return_t mr;
    mach_msg_size_t rcv_only_send_size;

    mr = mach_msg_trap(msg, option, send_size, rcv_size, rcv_name, timeout, notify);

    if (mr == MACH_SEND_INTERRUPTED) {
        if (option & MACH_SEND_INTERRUPT) {
            return MACH_SEND_INTERRUPTED;
        }

        do {
            mr = mach_msg_trap(msg, option, send_size, rcv_size, rcv_name, timeout, notify);
        } while (mr == MACH_SEND_INTERRUPTED);
    }

    if (mr == MACH_RCV_INTERRUPTED) {
        if (option & MACH_RCV_INTERRUPT) {
            return MACH_RCV_INTERRUPTED;
        }

        rcv_only_option = option & ~MACH_SEND_MSG;
        rcv_only_send_size = 0;

        do {
            mr = mach_msg_trap(
                msg, rcv_only_option, rcv_only_send_size, rcv_size, rcv_name, timeout, notify);
        } while (mr == MACH_RCV_INTERRUPTED);
    }

    return mr;
}
