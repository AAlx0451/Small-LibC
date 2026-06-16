#ifndef _MACH_MACH_SYSCALLS_H
#define _MACH_MACH_SYSCALLS_H

# include <features.h>

# if defined(_MACH_SOURCE)

#  include <mach/mach_traps.h>

#  define SYS_kernelrpc_mach_vm_allocate_trap         -10
#  define SYS_kernelrpc_mach_vm_deallocate_trap       -12
#  define SYS_kernelrpc_mach_vm_protect_trap          -14
#  define SYS_kernelrpc_mach_port_allocate_trap       -16
#  define SYS_kernelrpc_mach_port_destroy_trap        -17
#  define SYS_kernelrpc_mach_port_deallocate_trap     -18
#  define SYS_kernelrpc_mach_port_mod_refs_trap       -19
#  define SYS_kernelrpc_mach_port_move_member_trap    -20
#  define SYS_kernelrpc_mach_port_insert_right_trap   -21
#  define SYS_kernelrpc_mach_port_insert_member_trap  -22
#  define SYS_kernelrpc_mach_port_extract_member_trap -23
#  define SYS_mach_reply_port                         -26
#  define SYS_thread_self_trap                        -27
#  define SYS_task_self_trap                          -28
#  define SYS_host_self_trap                          -29
#  define SYS_mach_msg_trap                           -31
#  define SYS_mach_msg_overwrite_trap                 -32
#  define SYS_semaphore_signal_trap                   -33
#  define SYS_semaphore_signal_all_trap               -34
#  define SYS_semaphore_signal_thread_trap            -35
#  define SYS_semaphore_wait_trap                     -36
#  define SYS_semaphore_wait_signal_trap              -37
#  define SYS_semaphore_timedwait_trap                -38
#  define SYS_semaphore_timedwait_signal_trap         -39
#  define SYS_task_name_for_pid                       -44
#  define SYS_task_for_pid                            -45
#  define SYS_pid_for_task                            -46
#  define SYS_macx_swapon                             -48
#  define SYS_macx_swapoff                            -49
#  define SYS_macx_triggers                           -51
#  define SYS_macx_backing_store_suspend              -52
#  define SYS_macx_backing_store_recovery             -53
#  define SYS_pfz_exit                                -58
#  define SYS_swtch_pri                               -59
#  define SYS_swtch                                   -60
#  define SYS_thread_switch                           -61
#  define SYS_clock_sleep_trap                        -62
#  define SYS_mach_timebase_info_trap                 -89
#  define SYS_mach_wait_until_trap                    -90
#  define SYS_mk_timer_create_trap                    -91
#  define SYS_mk_timer_destroy_trap                   -92
#  define SYS_mk_timer_arm_trap                       -93
#  define SYS_mk_timer_cancel_trap                    -94
#  define SYS_iokit_user_client_trap                  -100

#  if defined(_C_SOURCE)

#   define _MSYS_GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#   define _MSYS_COUNT_ARGS(...)                                              _MSYS_GET_NTH_ARG(0, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#   define _MSYS_CONCAT_IMPL(name, count)                                     name##count
#   define _MSYS_CONCAT(name, count)                                          _MSYS_CONCAT_IMPL(name, count)

#   define mach_syscall(number, ...)                                                               \
       _MSYS_CONCAT(_mach_syscall, _MSYS_COUNT_ARGS(__VA_ARGS__))(number, ##__VA_ARGS__)

long _mach_syscall0(long number);
long _mach_syscall1(long number, long arg1);
long _mach_syscall2(long number, long arg1, long arg2);
long _mach_syscall3(long number, long arg1, long arg2, long arg3);
long _mach_syscall4(long number, long arg1, long arg2, long arg3, long arg4);
long _mach_syscall5(long number, long arg1, long arg2, long arg3, long arg4, long arg5);
long _mach_syscall6(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);

long _mach_syscall7(long number,
                    long arg1,
                    long arg2,
                    long arg3,
                    long arg4,
                    long arg5,
                    long arg6,
                    long arg7);

long _mach_syscall8(long number,
                    long arg1,
                    long arg2,
                    long arg3,
                    long arg4,
                    long arg5,
                    long arg6,
                    long arg7,
                    long arg8);

long _mach_syscall9(long number,
                    long arg1,
                    long arg2,
                    long arg3,
                    long arg4,
                    long arg5,
                    long arg6,
                    long arg7,
                    long arg8,
                    long arg9);

#  endif /* _C_SOURCE */

# endif /* _MACH_SOURCE */

#endif /* !_MACH_MACH_SYSCALLS_H */
