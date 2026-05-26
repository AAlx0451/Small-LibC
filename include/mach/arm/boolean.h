#ifndef MACH_ARM_BOOLEAN_H
#define MACH_ARM_BOOLEAN_H

#include <features.h>

#if defined(_MACH_SOURCE)

# if defined(__arm__) && defined(_C_SOURCE)
typedef int boolean_t;
# endif /* __arm__ && _C_SOURCE */

#endif /* _MACH_SOURCE */

#endif /* !MACH_ARM_BOOLEAN_H */
