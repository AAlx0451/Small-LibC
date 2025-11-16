#ifndef _ERRNO_H_
#define _ERRNO_H_

#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS   }
#else
# define __BEGIN_DECLS
# define __END_DECLS
#endif

__BEGIN_DECLS

int errno;

__END_DECLS


#define EDOM            33  /* Numerical argument out of domain */
#define ERANGE          34  /* Result too large */
#define EILSEQ          92  /* Illegal byte sequence */

#define EPERM           1   /* Operation not permitted */
#define ENOENT          2   /* No such file or directory */
#define ESRCH           3   /* No such process */
#define EINTR           4   /* Interrupted system call */
#define EIO             5   /* Input/output error */
#define ENXIO           6   /* Device not configured */
#define E2BIG           7   /* Argument list too long */
#define ENOEXEC         8   /* Exec format error */
#define EBADF           9   /* Bad file descriptor */
#define ECHILD          10  /* No child processes */
#define EAGAIN          35  /* Resource temporarily unavailable */
#define EWOULDBLOCK     EAGAIN /* Operation would block */
#define ENOMEM          12  /* Cannot allocate memory */
#define EACCES          13  /* Permission denied */
#define EFAULT          14  /* Bad address */
#define EBUSY           16  /* Device or resource busy */
#define EEXIST          17  /* File exists */
#define EXDEV           18  /* Cross-device link */
#define ENODEV          19  /* Operation not supported by device */
#define ENOTDIR         20  /* Not a directory */
#define EISDIR          21  /* Is a directory */
#define EINVAL          22  /* Invalid argument */
#define ENFILE          23  /* Too many open files in system */
#define EMFILE          24  /* Too many open files */
#define ENOTTY          25  /* Inappropriate ioctl for device */
#define EFBIG           27  /* File too large */
#define ENOSPC          28  /* No space left on device */
#define ESPIPE          29  /* Illegal seek */
#define EROFS           30  /* Read-only file system */
#define EMLINK          31  /* Too many links */
#define EPIPE           32  /* Broken pipe */
#define EDEADLK         11  /* Resource deadlock avoided */
#define ENOLCK          77  /* No locks available */
#define ENOSYS          78  /* Function not implemented */
#define ENOMSG          91  /* No message of desired type */
#define EIDRM           90  /* Identifier removed */
#define ENOTEMPTY       66  /* Directory not empty */
#define ENAMETOOLONG    63  /* File name too long */
#define ELOOP           62  /* Too many levels of symbolic links */
#define EOVERFLOW       84  /* Value too large to be stored in data type */
#define ECANCELED       89  /* Operation canceled */

#define EADDRINUSE      48  /* Address already in use */
#define EADDRNOTAVAIL   49  /* Can't assign requested address */
#define EAFNOSUPPORT    47  /* Address family not supported by protocol family */
#define EALREADY        37  /* Operation already in progress */
#define ECONNABORTED    53  /* Software caused connection abort */
#define ECONNREFUSED    61  /* Connection refused */
#define ECONNRESET      54  /* Connection reset by peer */
#define EDESTADDRREQ    39  /* Destination address required */
#define EHOSTUNREACH    65  /* No route to host */
#define EINPROGRESS     36  /* Operation now in progress */
#define EISCONN         56  /* Socket is already connected */
#define EMSGSIZE        40  /* Message too long */
#define ENETDOWN        50  /* Network is down */
#define ENETRESET       52  /* Network dropped connection on reset */
#define ENETUNREACH     51  /* Network is unreachable */
#define ENOBUFS         55  /* No buffer space available */
#define ENOPROTOOPT     42  /* Protocol not available */
#define ENOTCONN        57  /* Socket is not connected */
#define ENOTSOCK        38  /* Socket operation on non-socket */
#define EOPNOTSUPP      102 /* Operation not supported on socket */
#define ENOTSUP         45  /* Operation not supported (may be same as EOPNOTSUPP) */
#define EPROTO          100 /* Protocol error */
#define EPROTONOSUPPORT 43  /* Protocol not supported */
#define EPROTOTYPE      41  /* Protocol wrong type for socket */
#define ETIMEDOUT       60  /* Operation timed out */

#endif /* _ERRNO_H_ */
