#ifndef MACH_MACHINE_VM_PARAM_H
#define MACH_MACHINE_VM_PARAM_H

#include <features.h>

#if defined(_MACH_SOURCE)

#if defined(__arm__)
# include <mach/arm/vm_param.h>
#endif /* __arm__ */

#endif /* _MACH_SOURCE */

#endif /* !MACH_MACHINE_VM_PARAM_H */
