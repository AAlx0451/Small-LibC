#ifndef _MACH_VM_TYPES_H
#define _MACH_VM_TYPES_H

#include <features.h>

#if defined(_MACH_SOURCE)

# include <mach/machine/vm_types.h>
# include <mach/port.h>

# if defined(_C_SOURCE)

#  include <stdint.h>

typedef vm_offset_t pointer_t;    /* non-typed pointer */
typedef vm_offset_t vm_address_t; /* virtual memory address */

typedef uint64_t addr64_t; /* 64-bit address */
typedef uint32_t reg64_t;  /* 32-bit on 32-bit machines address */

typedef uint32_t ppnum_t; /* physical page number */

typedef mach_port_t vm_map_t; /* vm map pointer type */

#  define VM_MAP_NULL ((vm_map_t)0) /* NULL address */

typedef uint64_t vm_object_offset_t; /* offset in a vm object */
typedef uint64_t vm_object_size_t;   /* size of a vm object */

typedef mach_port_t upl_t;            /* universal page list port */
typedef mach_port_t vm_named_entry_t; /* named virtual memory entry port */

#  define UPL_NULL            ((upl_t)0)            /* upl NULL port */
#  define VM_NAMED_ENTRY_NULL ((vm_named_entry_t)0) /* virtual memory NULL entry port */

# endif /* _C_SOURCE */

# define PPNUM_MAX UINT32_MAX

#endif /* _MACH_SOURCE */

#endif /* !_MACH_VM_TYPES_H */
