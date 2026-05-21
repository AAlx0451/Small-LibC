#ifndef MACH_BOOLEAN_H
#define MACH_BOOLEAN_H

#include <features.h>
#if defined(_MACH_SOURCE)

#if defined(_C_SOURCE)
# include <mach/machine/boolean.h>
#endif /* _C_SOURCE */

#ifndef TRUE
# define TRUE 1
#endif /* !TRUE */

#ifndef FALSE
# define FALSE 0
#endif /* !FALSE */

#endif /* _MACH_SOURCE */

#endif /* !MACH_BOOLEAN_H */
