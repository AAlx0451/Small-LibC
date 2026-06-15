/*
 * XXX: much is not implemwnted yet
 */

#ifndef MACH_MACH_TYPES_H

# include <mach/clock_types.h>
# include <mach/host_info.h>
# include <mach/host_notify.h>
# include <mach/machine.h>
# include <mach/machine/vm_types.h>
# include <mach/message.h>
# include <mach/port.h>
# include <mach/time_value.h>
# include <mach/vm_statistics.h>
# include <mach/vm_types.h>

typedef mach_port_t task_t;
typedef mach_port_t task_name_t;
typedef mach_port_t thread_t;
typedef mach_port_t thread_act_t;
typedef mach_port_t ipc_space_t;
typedef mach_port_t host_t;
typedef mach_port_t host_priv_t;
typedef mach_port_t host_security_t;
typedef mach_port_t processor_t;
typedef mach_port_t processor_set_t;
typedef mach_port_t processor_set_control_t;
typedef mach_port_t semaphore_t;
typedef mach_port_t lock_set_t;
typedef mach_port_t ledger_t;
typedef mach_port_t alarm_t;
typedef mach_port_t clock_serv_t;
typedef mach_port_t clock_ctrl_t;
typedef mach_port_t semaphore_t;
typedef mach_port_t clock_reply_t;
typedef mach_port_t bootstrap_t;
typedef mach_port_t mem_entry_name_port_t;
typedef mach_port_t exception_handler_t;
typedef mach_port_t vm_task_entry_t;
typedef mach_port_t io_master_t;
typedef mach_port_t UNDServerRef;

typedef exception_handler_t *exception_handler_array_t;
typedef processor_set_t processor_set_name_t;
typedef task_t *task_array_t;
typedef thread_t *thread_array_t;
typedef processor_set_t *processor_set_array_t;
typedef processor_set_t *processor_set_name_array_t;
typedef processor_t *processor_array_t;
typedef thread_act_t *thread_act_array_t;
typedef ledger_t *ledger_array_t;

typedef task_t task_port_t;
typedef task_array_t task_port_array_t;
typedef thread_t thread_port_t;
typedef thread_array_t thread_port_array_t;
typedef ipc_space_t ipc_space_port_t;
typedef host_t host_name_t;
typedef host_t host_name_port_t;
typedef processor_set_t processor_set_port_t;
typedef processor_set_t processor_set_name_port_t;
typedef processor_set_array_t processor_set_name_port_array_t;
typedef processor_set_t processor_set_control_port_t;
typedef processor_t processor_port_t;
typedef processor_array_t processor_port_array_t;
typedef thread_act_t thread_act_port_t;
typedef thread_act_array_t thread_act_port_array_t;
typedef semaphore_t semaphore_port_t;
typedef lock_set_t lock_set_port_t;
typedef ledger_t ledger_port_t;
typedef ledger_array_t ledger_port_array_t;
typedef alarm_t alarm_port_t;
typedef clock_serv_t clock_serv_port_t;
typedef clock_ctrl_t clock_ctrl_port_t;
typedef exception_handler_t exception_port_t;
typedef exception_handler_array_t exception_port_arrary_t;
typedef natural_t ledger_item_t;
typedef int64_t ledger_amount_t;
typedef mach_vm_offset_t *emulation_vector_t;
typedef char *user_subsystem_t;
typedef char *labelstr_t;

# define TASK_NULL          ((task_t)0)
# define TASK_NAME_NULL     ((task_name_t)0)
# define THREAD_NULL        ((thread_t)0)
# define TID_NULL           ((uint64_t)0)
# define THR_ACT_NULL       ((thread_act_t)0)
# define IPC_SPACE_NULL     ((ipc_space_t)0)
# define HOST_NULL          ((host_t)0)
# define HOST_PRIV_NULL     ((host_priv_t)0)
# define HOST_SECURITY_NULL ((host_security_t)0)
# define PROCESSOR_SET_NULL ((processor_set_t)0)
# define PROCESSOR_NULL     ((processor_t)0)
# define SEMAPHORE_NULL     ((semaphore_t)0)
# define LOCK_SET_NULL      ((lock_set_t)0)
# define LEDGER_NULL        ((ledger_t)0)
# define ALARM_NULL         ((alarm_t)0)
# define CLOCK_NULL         ((clock_t)0)
# define UND_SERVER_NULL    ((UNDServerRef)0)

# define LEDGER_ITEM_INFINITY  ((ledger_item_t)(~0))
# define LEDGER_LIMIT_INFINITY ((ledger_amount_t)(((uint64_t)1 << 63) - 1))

#endif /* !MACH_MACH_TYPES_H */
