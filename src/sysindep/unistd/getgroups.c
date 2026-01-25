#include <unistd.h>
#include <sys/syscall.h>
int getgroups(int gidsetsize, gid_t grouplist[]) {
    return (int)syscall(SYS_getgroups, gidsetsize, (long)grouplist);
}
