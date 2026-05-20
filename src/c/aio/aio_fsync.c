#include <aio.h>
#include <sys/syscall.h>
#include <unistd.h>

int aio_fsync(int op, struct aiocb *aiocbp)
{
    return (int)syscall(SYS_aio_fsync, (long)op, (long)aiocbp);
}
