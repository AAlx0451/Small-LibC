#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#if !defined(SYS_getpid) && defined(__NR_getpid)
#define SYS_getpid __NR_getpid
#elif !defined(SYS_getpid)
#error "Please include a proper syscall.h. See syscall.h"
#endif

pid_t getpid(void) {
    long ret;
    ret = syscall0(SYS_getpid);
    return (pid_t)ret;
}
