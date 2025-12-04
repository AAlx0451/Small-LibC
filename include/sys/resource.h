#ifndef SYS_RESOURCE_H
# define SYS_RESOURCE_H

# include<sys/time.h>

struct  rusage {
        struct timeval ru_utime;        /* user time used (PL) */
        struct timeval ru_stime;        /* system time used (PL) */                                 
        long    ru_maxrss;              /* max resident set size (PL) */
#define ru_first        ru_ixrss        /* internal: ruadd() range start */
        long    ru_ixrss;               /* integral shared memory size (NU) */
        long    ru_idrss;               /* integral unshared data (NU)  */
        long    ru_isrss;               /* integral unshared stack (NU) */
        long    ru_minflt;              /* page reclaims (NU) */
        long    ru_majflt;              /* page faults (NU) */
        long    ru_nswap;               /* swaps (NU) */
        long    ru_inblock;             /* block input operations (atomic) */
        long    ru_oublock;             /* block output operations (atomic) */
        long    ru_msgsnd;              /* messages sent (atomic) */
        long    ru_msgrcv;              /* messages received (atomic) */
        long    ru_nsignals;            /* signals received (atomic) */
        long    ru_nvcsw;               /* voluntary context switches (atomic) */
        long    ru_nivcsw;              /* involuntary " */
#define ru_last         ru_nivcsw       /* internal: ruadd() range end */
};

#endif
