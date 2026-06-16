#ifndef _MACH_MACHINE_KERN_RETURN_H
#define _MACH_MACHINE_KERN_RETURN_H

#include <features.h>

#if defined(_MACH_SOURCE)

# if defined(__arm__)
#  include <mach/arm/kern_return.h>
# endif /* __arm__ */

#endif /* _MACH_SOURCE */

#endif /* !_MACH_MACHINE_KERN_RETURN_H */
