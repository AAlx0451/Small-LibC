#ifndef MACH_MACHINE_BOOLEAN_H
#define MACH_MACHINE_BOOLEAN_H

#include <features.h>

#if defined(_MACH_SOURCE)

# if defined(__arm__)
#  include <mach/arm/boolean.h>
# endif /* __arm__ */

#endif /* _MACH_SOURCE */

#endif /* !MACH_MACHINE_BOOLEAN_H */
