#include <unistd.h>
#include <sys/syscall.h>
#include <sys/resource.h>

int getrusage(int who, struct rusage *usage) {
    long ret = syscall(SYS_getrusage, who, (long)usage);
    return (int)ret;
}
