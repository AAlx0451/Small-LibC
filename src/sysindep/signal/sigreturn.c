#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>

int sigreturn(void *uctx, int infostyle) {
    return (int)syscall(SYS_sigreturn, (long)uctx, (long)infostyle);
}
