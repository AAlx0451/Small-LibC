#include <errno.h>
#include <unistd.h>

#define SYS_getrlimit 194
#define SYS___sysctl 202

#define CTL_KERN 1
#define CTL_HW 6

#define KERN_ARGMAX 8
#define KERN_CLOCKRATE 12
#define KERN_NGROUPS 18

#define HW_PAGESIZE 7

#define RLIMIT_NPROC 7
#define RLIMIT_NOFILE 8

typedef unsigned long long rlim_t;

struct rlimit {
    rlim_t rlim_cur;
    rlim_t rlim_max;
};

struct clockinfo {
    int hz;
    int tick;
    int tickadj;
    int stathz;
    int profhz;
};

long sysconf(int name) {
    int mib[2];
    size_t len;
    int int_val;
    struct clockinfo clk_info;
    struct rlimit rl;

    switch(name) {
    case _SC_ARG_MAX:
        mib[0] = CTL_KERN;
        mib[1] = KERN_ARGMAX;
        len = sizeof(int_val);
        if(syscall(SYS___sysctl, (long)mib, (long)2, (long)&int_val, (long)&len, (long)NULL, (long)0) == -1)
            return -1;
        return (long)int_val;

    case _SC_CHILD_MAX:
        if(syscall(SYS_getrlimit, (long)RLIMIT_NPROC, (long)&rl) == -1)
            return -1;
        if(rl.rlim_cur == 0x7FFFFFFFFFFFFFFFLL)
            return -1;
        return (long)rl.rlim_cur;

    case _SC_CLK_TCK:
        mib[0] = CTL_KERN;
        mib[1] = KERN_CLOCKRATE;
        len = sizeof(clk_info);
        if(syscall(SYS___sysctl, (long)mib, (long)2, (long)&clk_info, (long)&len, (long)NULL, (long)0) == -1)
            return 100;
        return (long)clk_info.hz;

    case _SC_NGROUPS_MAX:
        mib[0] = CTL_KERN;
        mib[1] = KERN_NGROUPS;
        len = sizeof(int_val);
        if(syscall(SYS___sysctl, (long)mib, (long)2, (long)&int_val, (long)&len, (long)NULL, (long)0) == -1)
            return -1;
        return (long)int_val;

    case _SC_OPEN_MAX:
        if(syscall(SYS_getrlimit, (long)RLIMIT_NOFILE, (long)&rl) == -1)
            return 256;
        if(rl.rlim_cur == 0x7FFFFFFFFFFFFFFFLL)
            return -1;
        return (long)rl.rlim_cur;

    case _SC_JOB_CONTROL:
        return 1;

    case _SC_SAVED_IDS:
        return 1;

    case _SC_VERSION:
        return 199009L;

    case _SC_BC_BASE_MAX:
        return 99;

    case _SC_BC_DIM_MAX:
        return 2048;

    case _SC_BC_SCALE_MAX:
        return 99;

    case _SC_BC_STRING_MAX:
        return 1000;

    case _SC_COLL_WEIGHTS_MAX:
        return 2;

    case _SC_EXPR_NEST_MAX:
        return 32;

    case _SC_LINE_MAX:
        return 2048;

    case _SC_RE_DUP_MAX:
        return 255;

    case _SC_2_VERSION:
        return 199209L;

    case _SC_2_C_BIND:
        return 200112L;

    case _SC_2_C_DEV:
        return -1;

    case _SC_2_CHAR_TERM:
        return 1;

    case _SC_2_FORT_DEV:
        return -1;

    case _SC_2_FORT_RUN:
        return -1;

    case _SC_2_LOCALEDEF:
        return -1;

    case _SC_2_SW_DEV:
        return -1;

    case _SC_2_UPE:
        return -1;

    case _SC_STREAM_MAX:
        return 256;

    case _SC_TZNAME_MAX:
        return 255;

    case _SC_ASYNCHRONOUS_IO:
        return -1;

    case _SC_PAGESIZE:
        mib[0] = CTL_HW;
        mib[1] = HW_PAGESIZE;
        len = sizeof(int_val);
        if(syscall(SYS___sysctl, (long)mib, (long)2, (long)&int_val, (long)&len, (long)NULL, (long)0) == -1)
            return 4096;
        return (long)int_val;

    default:
        errno = 22;
        return -1;
    }
}
