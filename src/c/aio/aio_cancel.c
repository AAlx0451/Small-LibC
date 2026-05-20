#include <aio.h>
#include <sys/syscall.h>
#include <unistd.h>

int aio_cancel(int fildes, struct aiocb *aiocbp)
{
    return (int)syscall(SYS_aio_cancel, (long)fildes, (long)aiocbp);
}
