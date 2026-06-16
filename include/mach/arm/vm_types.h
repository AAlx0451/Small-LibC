#ifndef _MACH_ARM_VM_TYPES_H
#define _MACH_ARM_VM_TYPES_H

#include <features.h>

#if defined(_MACH_SOURCE)

# if defined(_C_SOURCE)

#  include <mach/arm/vm_param.h>
#  include <stdint.h>

typedef unsigned natural_t; /* XXX: natural_t and integer_t are deprecated */
typedef int integer_t;      /* signed natural_t */

typedef natural_t vm_offset_t; /* offset or non-typed pointer */
typedef natural_t vm_size_t;   /* memory size */

typedef uint64_t mach_vm_address_t; /* 64 bit virtual address */
typedef uint64_t mach_vm_offset_t;  /* 64-bit offset or non-typed pointer */
typedef uint64_t mach_vm_size_t;    /* 64-bit virtual memory size */

typedef uint32_t vm_map_address_t; /* virtual address in VM map structures */
typedef uint32_t vm_map_offset_t;  /* offset or non-typed pointer in VM map structures */
typedef uint32_t vm_map_size_t;    /* virtual memory size in VM map structures */

typedef uint32_t vm32_address_t; /* 32-bit virtual address */
typedef uint32_t vm32_offset_t;  /* 32-bit offset or non-typed pointer */
typedef uint32_t vm32_size_t;    /* 32-bit virtual memory size */

typedef vm_offset_t mach_port_context_t; /* pointer-sized token for port association/guarding */

#  define VM_MAP_MIN_ADDRESS VM_MIN_ADDRESS
#  define VM_MAP_MAX_ADDRESS VM_MAX_ADDRESS

# endif /*_C_SOURCE */

# define MACH_MSG_TYPE_INTEGER_T MACH_MSG_TYPE_INTEGER_32

#endif /* _MACH_SOURCE */

#endif /* !_MACH_ARM_VM_TYPES_H */
