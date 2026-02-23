#ifndef SYS_LOCK_H
#define SYS_LOCK_H

#include <features.h>
#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_XOPEN_SOURCE))

#define UNLOCK  0
#define PROCLOCK 1
#define TXTLOCK 2
#define DATLOCK 4

int plock(int);

#endif /* !_ANSI && (_DARWIN_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !SYS_LOCK_H */
