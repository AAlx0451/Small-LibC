#ifndef _SCHED_H
#define _SCHED_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L)

struct sched_param {
    int sched_priority;
# if defined(_DARWIN_C_SOURCE)
    int quantum;
# else
    char opaque[sizeof(int)];
# endif /* _DARWIN_C_SOURCE */
};

int sched_yield(void);

#endif /* !_ANSI && (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 199309L) */

#endif /* !_SCHED_H */
