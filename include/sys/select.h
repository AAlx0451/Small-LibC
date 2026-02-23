#ifndef SYS_SELECT_H
#define SYS_SELECT_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)

#include <sys/time.h>

# define FD_SETSIZE 1024
# define NBBY 8
# define NFDBITS (sizeof(int) * NBBY)
# define howmany(x, y) ((((x) % (y)) == 0) ? ((x) / (y)) : (((x) / (y)) + 1))

typedef struct fd_set {
    int fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

#endif /* !_ANSI && (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) */

#endif /* !SYS_SELECT_H */
