#include <aio.h>
#include <sys/syscall.h>
#include <unistd.h>

int aio_write(struct aiocb *aiocbp) { return (int)syscall(SYS_aio_write, (long)aiocbp); }
