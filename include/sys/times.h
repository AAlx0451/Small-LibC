#ifndef SYS_TIMES_H
#define SYS_TIMES_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))

#include <sys/types.h>

struct tms {
    clock_t tms_utime; 
    clock_t tms_stime; 
    clock_t tms_cutime;
    clock_t tms_cstime;
};

clock_t times(struct tms *buffer);

#endif /* !_ANSI && (_POSIX_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !SYS_TIMES_H */
