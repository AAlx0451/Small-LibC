#include <aio.h>
#include <sys/syscall.h>
#include <unistd.h>

int aio_error(struct aiocb *aiocbp) { return (int)syscall(SYS_aio_error, (long)aiocbp); }
