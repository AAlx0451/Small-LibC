#ifndef _MACH_MACHINE_VM_PARAM_H
#define _MACH_MACHINE_VM_PARAM_H

#include <features.h>

#if defined(_MACH_SOURCE)

# if defined(__arm__)
#  include <mach/arm/vm_param.h>
# endif /* __arm__ */

#endif /* _MACH_SOURCE */

#endif /* !_MACH_MACHINE_VM_PARAM_H */
