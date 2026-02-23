#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>

int getrusage(int who, struct rusage *usage) {
    long ret = syscall(SYS_getrusage, who, (long)usage);
    return (int)ret;
}
