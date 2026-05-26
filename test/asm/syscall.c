#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

#define PASS GREEN("[PASS]")
#define FAIL RED("[FAIL]")

void syscall_test()
{
    if (syscall(SYS_getgid) >= 0)
        printf(PASS " syscall(SYS_getgid) >= 0\n");
    else
        printf(FAIL " syscall(SYS_getgid) > 0\n");

    int fd = open("/dev/null", O_RDWR);
    if (syscall(SYS_write, fd, (long)"test", 4) == 4)
        printf(PASS " syscall(SYS_write, fd, \"test\", 4) == 4\n");
    else
        printf(FAIL " syscall(SYS_write, fd, \"test\", 4) == 4\n");

    signal(SIGSYS, SIG_IGN);
    syscall(99999);
    if (errno == ENOSYS)
        printf(PASS " errno == ENOSYS after syscall >= MAXSYSCALL\n");
    else
        printf(FAIL "errno == ENOSYS after syscall >= MAXSYSCALL\n");

    if (syscall(SYS_read, -42, (long)"", 10) == -1)
        printf(PASS " syscall() returns -1 if error\n");
    else
        printf(FAIL " syscall() returns -1 if error\n");
}
