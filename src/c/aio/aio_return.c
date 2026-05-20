#include <aio.h>
#include <sys/syscall.h>
#include <unistd.h>

ssize_t aio_return(struct aiocb *aiocbp) { return (ssize_t)syscall(SYS_aio_return, (long)aiocbp); }
