#ifndef _MACH_HOST_INFO_H
#define _MACH_HOST_INFO_H

#include <features.h>

#if defined(_MACH_SOURCE)

# define KERNEL_VERSION_MAX   (512)
# define HOST_INFO_MAX        (1024)
# define KERNEL_BOOT_INFO_MAX (4096)

# define HOST_LOAD_INFO         1
# define HOST_VM_INFO           2
# define HOST_CPU_LOAD_INFO     3
# define HOST_VM_INFO64         4
# define HOST_EXTMOD_INFO64     5
# define HOST_EXPIRED_TASK_INFO 6

# define HOST_BASIC_INFO      1
# define HOST_SCHED_INFO      3
# define HOST_RESOURCE_SIZES  4
# define HOST_PRIORITY_INFO   5
# define HOST_SEMAPHORE_TRAPS 7
# define HOST_MACH_MSG_TRAP   8

# if defined(_C_SOURCE)

#  include <mach/machine.h>
#  include <mach/machine/vm_types.h>
#  include <mach/message.h>
#  include <mach/time_value.h>
#  include <mach/vm_statistics.h>

typedef integer_t *host_info_t;
typedef integer_t *host_info64_t;
typedef integer_t host_info_data_t[HOST_INFO_MAX];
typedef char kernel_version_t[KERNEL_VERSION_MAX];
typedef char kernel_boot_info_t[KERNEL_BOOT_INFO_MAX];
typedef integer_t host_flavor_t;

#  pragma pack(4)

struct host_basic_info {
    integer_t max_cpus;
    integer_t avail_cpus;
    natural_t memory_size;
    cpu_type_t cpu_type;
    cpu_subtype_t cpu_subtype;
    cpu_threadtype_t cpu_threadtype;
    integer_t physical_cpu;
    integer_t physical_cpu_max;
    integer_t logical_cpu;
    integer_t logical_cpu_max;
    uint64_t max_mem;
};

#  pragma pack()

typedef struct host_basic_info host_basic_info_data_t;
typedef struct host_basic_info *host_basic_info_t;
#  define HOST_BASIC_INFO_COUNT                                                                    \
      ((mach_msg_type_number_t)(sizeof(host_basic_info_data_t) / sizeof(integer_t)))

struct host_sched_info {
    integer_t min_timeout;
    integer_t min_quantum;
};

typedef struct host_sched_info host_sched_info_data_t;
typedef struct host_sched_info *host_sched_info_t;
#  define HOST_SCHED_INFO_COUNT                                                                    \
      ((mach_msg_type_number_t)(sizeof(host_sched_info_data_t) / sizeof(integer_t)))

struct kernel_resource_sizes {
    natural_t task;
    natural_t thread;
    natural_t port;
    natural_t memory_region;
    natural_t memory_object;
};

typedef struct kernel_resource_sizes kernel_resource_sizes_data_t;
typedef struct kernel_resource_sizes *kernel_resource_sizes_t;
#  define HOST_RESOURCE_SIZES_COUNT                                                                \
      ((mach_msg_type_number_t)(sizeof(kernel_resource_sizes_data_t) / sizeof(integer_t)))

struct host_priority_info {
    integer_t kernel_priority;
    integer_t system_priority;
    integer_t server_priority;
    integer_t user_priority;
    integer_t depress_priority;
    integer_t idle_priority;
    integer_t minimum_priority;
    integer_t maximum_priority;
};

typedef struct host_priority_info host_priority_info_data_t;
typedef struct host_priority_info *host_priority_info_t;
#  define HOST_PRIORITY_INFO_COUNT                                                                 \
      ((mach_msg_type_number_t)(sizeof(host_priority_info_data_t) / sizeof(integer_t)))

struct host_load_info {
    integer_t avenrun[3];
    integer_t mach_factor[3];
};

typedef struct host_load_info host_load_info_data_t;
typedef struct host_load_info *host_load_info_t;

#  define HOST_LOAD_INFO_COUNT                                                                     \
      ((mach_msg_type_number_t)(sizeof(host_load_info_data_t) / sizeof(integer_t)))

#  define HOST_VM_INFO64_COUNT                                                                     \
      ((mach_msg_type_number_t)(sizeof(vm_statistics64_data_t) / sizeof(integer_t)))
#  define HOST_VM_INFO64_LATEST_COUNT HOST_VM_INFO64_COUNT

#  define HOST_EXTMOD_INFO64_COUNT                                                                 \
      ((mach_msg_type_number_t)(sizeof(vm_extmod_statistics_data_t) / sizeof(integer_t)))
#  define HOST_EXTMOD_INFO64_LATEST_COUNT HOST_EXTMOD_INFO64_COUNT

#  define HOST_VM_INFO_COUNT                                                                       \
      ((mach_msg_type_number_t)(sizeof(vm_statistics_data_t) / sizeof(integer_t)))
#  define HOST_VM_INFO_LATEST_COUNT HOST_VM_INFO_COUNT
#  define HOST_VM_INFO_REV2_COUNT   HOST_VM_INFO_LATEST_COUNT
#  define HOST_VM_INFO_REV1_COUNT   ((mach_msg_type_number_t)(HOST_VM_INFO_REV2_COUNT - 1))
#  define HOST_VM_INFO_REV0_COUNT   ((mach_msg_type_number_t)(HOST_VM_INFO_REV1_COUNT - 2))

struct host_cpu_load_info {
    natural_t cpu_ticks[CPU_STATE_MAX];
};

typedef struct host_cpu_load_info host_cpu_load_info_data_t;
typedef struct host_cpu_load_info *host_cpu_load_info_t;
#  define HOST_CPU_LOAD_INFO_COUNT                                                                 \
      ((mach_msg_type_number_t)(sizeof(host_cpu_load_info_data_t) / sizeof(integer_t)))

# endif /* _C_SOURCE */

#endif /* _MACH_SOURCE */

#endif /* !_MACH_HOST_INFO_H */
