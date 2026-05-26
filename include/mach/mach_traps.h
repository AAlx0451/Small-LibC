#ifndef MACH_MACH_TRAPS_H
#define MACH_MACH_TRAPS_H

#include <features.h>

#define __TODO 0

#if defined(_MACH_SOURCE)

# include <mach/std_types.h>
# include <stdint.h>
# if __TODO
#  include <mach/clock_types.h>
#  include <mach/mach_types.h>
#  include <machine/endian.h>
# endif /* __TODO */
# include <mach/kern_return.h>
# include <mach/port.h>
# include <mach/vm_types.h>

# if defined(_C_SOURCE)

extern mach_port_name_t mach_reply_port(void);

extern mach_port_name_t thread_self_trap(void);

extern mach_port_name_t host_self_trap(void);

#  if __TODO
extern mach_msg_return_t mach_msg_trap(mach_msg_header_t *msg,
                                       mach_msg_option_t option,
                                       mach_msg_size_t send_size,
                                       mach_msg_size_t rcv_size,
                                       mach_port_name_t rcv_name,
                                       mach_msg_timeout_t timeout,
                                       mach_port_name_t notify);

extern mach_msg_return_t mach_msg_overwrite_trap(mach_msg_header_t *msg,
                                                 mach_msg_option_t option,
                                                 mach_msg_size_t send_size,
                                                 mach_msg_size_t rcv_size,
                                                 mach_port_name_t rcv_name,
                                                 mach_msg_timeout_t timeout,
                                                 mach_port_name_t notify,
                                                 mach_msg_header_t *rcv_msg,
                                                 mach_msg_size_t rcv_limit);
#  endif /* __TODO */

extern kern_return_t semaphore_signal_trap(mach_port_name_t signal_name);

extern kern_return_t semaphore_signal_all_trap(mach_port_name_t signal_name);

extern kern_return_t semaphore_signal_thread_trap(mach_port_name_t signal_name,
                                                  mach_port_name_t thread_name);

extern kern_return_t semaphore_wait_trap(mach_port_name_t wait_name);

extern kern_return_t semaphore_wait_signal_trap(mach_port_name_t wait_name,
                                                mach_port_name_t signal_name);

#  if __TODO
extern kern_return_t semaphore_timedwait_trap(mach_port_name_t wait_name,
                                              unsigned int sec,
                                              clock_res_t nsec);

extern kern_return_t semaphore_timedwait_signal_trap(mach_port_name_t wait_name,
                                                     mach_port_name_t signal_name,
                                                     unsigned int sec,
                                                     clock_res_t nsec);

extern kern_return_t clock_sleep_trap(mach_port_name_t clock_name,
                                      sleep_type_t sleep_type,
                                      int sleep_sec,
                                      int sleep_nsec,
                                      mach_timespec_t *wakeup_time);
#  endif /* __TODO */

extern kern_return_t _kernelrpc_mach_vm_allocate_trap(mach_port_name_t target,
                                                      mach_vm_offset_t *addr,
                                                      mach_vm_size_t size,
                                                      int flags);

extern kern_return_t _kernelrpc_mach_vm_deallocate_trap(mach_port_name_t target,
                                                        mach_vm_address_t address,
                                                        mach_vm_size_t size);

#  if __TODO
extern kern_return_t _kernelrpc_mach_vm_protect_trap(mach_port_name_t target,
                                                     mach_vm_address_t address,
                                                     mach_vm_size_t size,
                                                     boolean_t set_maximum,
                                                     vm_prot_t new_protection);
#  endif /* __TODO */

extern kern_return_t _kernelrpc_mach_port_allocate_trap(mach_port_name_t target,
                                                        mach_port_right_t right,
                                                        mach_port_name_t *name);

extern kern_return_t _kernelrpc_mach_port_destroy_trap(mach_port_name_t target,
                                                       mach_port_name_t name);

extern kern_return_t _kernelrpc_mach_port_deallocate_trap(mach_port_name_t target,
                                                          mach_port_name_t name);

extern kern_return_t _kernelrpc_mach_port_mod_refs_trap(mach_port_name_t target,
                                                        mach_port_name_t name,
                                                        mach_port_right_t right,
                                                        mach_port_delta_t delta);

extern kern_return_t _kernelrpc_mach_port_move_member_trap(mach_port_name_t target,
                                                           mach_port_name_t member,
                                                           mach_port_name_t after);

#  if __TODO
extern kern_return_t _kernelrpc_mach_port_insert_right_trap(mach_port_name_t target,
                                                            mach_port_name_t name,
                                                            mach_port_name_t poly,
                                                            mach_msg_type_name_t polyPoly);
#  endif /* __TODO */

extern kern_return_t _kernelrpc_mach_port_insert_member_trap(mach_port_name_t target,
                                                             mach_port_name_t name,
                                                             mach_port_name_t pset);

extern kern_return_t _kernelrpc_mach_port_extract_member_trap(mach_port_name_t target,
                                                              mach_port_name_t name,
                                                              mach_port_name_t pset);

extern kern_return_t macx_swapon(uint64_t filename, int flags, int size, int priority);

extern kern_return_t macx_swapoff(uint64_t filename, int flags);

extern kern_return_t macx_triggers(int hi_water, int low_water, int flags, mach_port_t alert_port);

extern kern_return_t macx_backing_store_suspend(boolean_t suspend);

extern kern_return_t macx_backing_store_recovery(int pid);

extern boolean_t swtch_pri(int pri);

extern boolean_t swtch(void);

#  if __TODO
extern kern_return_t thread_switch(mach_port_name_t thread_name,
                                   int option,
                                   mach_msg_timeout_t option_time);
#  endif /* __TODO */

extern mach_port_name_t task_self_trap(void);

extern kern_return_t task_for_pid(mach_port_name_t target_tport, int pid, mach_port_name_t *t);

extern kern_return_t task_name_for_pid(mach_port_name_t target_tport,
                                       int pid,
                                       mach_port_name_t *tn);

extern kern_return_t pid_for_task(mach_port_name_t t, int *x);

# endif /* _C_SOURCE */

#endif /* _MACH_SOURCE */

#endif /* !MACH_MACH_TRAPS_H */
