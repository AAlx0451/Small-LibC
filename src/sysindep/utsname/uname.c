#include <sys/utsname.h>
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/syscall.h>

#define CTL_KERN        1
#define CTL_HW          6

#define KERN_OSTYPE     1  /* sysname */
#define KERN_OSRELEASE  2  /* release */
#define KERN_OSREV      3
#define KERN_VERSION    4
#define KERN_HOSTNAME   10 /* nodename */

#define HW_MACHINE      1  /* machine */ 

static int get_sysctl_string(int major, int minor, char *buf, size_t maxlen) {
    int mib[2];
    size_t len;
    int ret;

    mib[0] = major;
    mib[1] = minor;
    len = maxlen;

    /* 
     * sysctl(int *name, u_int namelen, void *oldp, size_t *oldlenp, 
     *        void *newp, size_t newlen);
     */
    ret = syscall(SYS___sysctl, (long)mib, 2, (long)buf, (long)&len, (long)NULL, 0);

    if (ret == 0) {
        if (len < maxlen) {
            buf[len] = '\0';
        } else {
            buf[maxlen - 1] = '\0';
        }
    }
    return ret;
}

int uname(struct utsname *name) {
    if (name == NULL) {
        return -1; // EFAULT
    }

    int res = 0;

    if (get_sysctl_string(CTL_KERN, KERN_OSTYPE, name->sysname, 256) != 0)
        res = -1;

    if (get_sysctl_string(CTL_KERN, KERN_HOSTNAME, name->nodename, 256) != 0)
        res = -1;

    if (get_sysctl_string(CTL_KERN, KERN_OSRELEASE, name->release, 256) != 0)
        res = -1;

    if (get_sysctl_string(CTL_KERN, KERN_VERSION, name->version, 256) != 0)
        res = -1;

    if (get_sysctl_string(CTL_HW, HW_MACHINE, name->machine, 256) != 0)
        res = -1;

    return res;
}
