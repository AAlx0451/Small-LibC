#ifndef SYS_ACCT_H
#define SYS_ACCT_H

#include <sys/types.h>

typedef uint16_t comp_t;

#define AFORK 0x01
#define ASU 0x02
#define ACOMPAT 0x04
#define ACORE 0x08
#define AXSIG 0x10
#define AHZ 64

struct acct {
    char ac_comm[10];
    comp_t ac_utime;
    comp_t ac_stime;
    comp_t ac_etime;
    uint32_t ac_btime;
    uid_t ac_uid;
    gid_t ac_gid;
    uint16_t ac_mem;
    comp_t ac_io;
    dev_t ac_tty;
    uint8_t ac_flag;
};

#endif /* SYS_ACCT_H */

