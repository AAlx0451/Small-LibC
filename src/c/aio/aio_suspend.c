#include <aio.h>
#include <sys/syscall.h>
#include <unistd.h>

int aio_suspend(const struct aiocb *const list[], int nent, const struct timespec *timeout)
{
    return (int)syscall(SYS_aio_suspend, (long)list, (long)nent, (long)timeout);
}
