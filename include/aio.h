#ifndef AIO_H
#define AIO_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L)

# include <signal.h>
# include <sys/types.h>
# include <time.h>

struct aiocb {
    int aio_fildes;
    off_t aio_offset;
    volatile void *aio_buf;
    size_t aio_nbytes;
    int aio_reqprio;
    struct sigevent aio_sigevent;
    int aio_lio_opcode;
};

# define AIO_CANCELED    0x2
# define AIO_NOTCANCELED 0x4
# define AIO_ALLDONE     0x1
# define LIO_WAIT        0x2
# define LIO_NOWAIT      0x1
# define LIO_READ        0x1
# define LIO_WRITE       0x2
# define LIO_NOP         0x0

int aio_read(struct aiocb *aiocbp);
int aio_write(struct aiocb *aiocbp);
int lio_listio(int mode, struct aiocb *const list[], int nent, struct sigevent *sig);
int aio_error(struct aiocb *aiocbp);
ssize_t aio_return(struct aiocb *aiocbp);
int aio_cancel(int fildes, struct aiocb *aiocbp);
int aio_suspend(const struct aiocb *const list[], int nent, const struct timespec *timeout);
int aio_fsync(int op, struct aiocb *aiocbp);

#endif /* !_ANSI && (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 199309L) */

#endif /* !AIO_H */
