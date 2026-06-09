#ifndef MACH_SYNC_POLICY_H
#define MACH_SYNC_POLICY_H

#include <features.h>

#if defined(_MACH_SOURCE)

# define SYNC_POLICY_FIFO           0x0
# define SYNC_POLICY_FIXED_PRIORITY 0x1
# define SYNC_POLICY_REVERSED       0x2
# define SYNC_POLICY_ORDER_MASK     0x3
# define SYNC_POLICY_LIFO           (SYNC_POLICY_FIFO | SYNC_POLICY_REVERSED)

# define SYNC_POLICY_MAX 0x7

# if defined(_C_SOURCE)

typedef int sync_policy_t;

# endif /* _C_SOURCE */

#endif /* _MACH_SOURCE */

#endif /* !MACH_SYNC_POLICY_H */
