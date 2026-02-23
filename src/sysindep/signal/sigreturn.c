#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>

int sigreturn(void *uctx, int infostyle) {
    return (int)syscall(SYS_sigreturn, (long)uctx, (long)infostyle);
}
