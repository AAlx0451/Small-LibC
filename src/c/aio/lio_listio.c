#include <aio.h>
#include <sys/syscall.h>
#include <unistd.h>

int lio_listio(int mode, struct aiocb *const list[], int nent, struct sigevent *sig)
{
    return (int)syscall(SYS_lio_listio, mode, (long)list, nent, (long)sig);
}
