#include <aio.h>
#include <sys/syscall.h>
#include <unistd.h>

int aio_read(struct aiocb *aiocbp) { return (int)syscall(SYS_aio_read, (long)aiocbp); }
