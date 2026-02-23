#ifndef ERRNO_H
#define ERRNO_H

#include <features.h>

extern int errno;

#define EDOM 33
#define ERANGE 34

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409L) || defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
# define EILSEQ 92
#endif /* (__STDC_VERSION__ >= 199409L) || _POSIX_C_SOURCE || _DARWIN_C_SOURCE */

#if !defined(_ANSI)
# if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) || defined(_DARWIN_C_SOURCE)
#  define EPERM 1
#  define ENOENT 2
#  define ESRCH 3
#  define EINTR 4
#  define EIO 5
#  define ENXIO 6
#  define E2BIG 7
#  define ENOEXEC 8
#  define EBADF 9
#  define ECHILD 10
#  define EDEADLK 11
#  define ENOMEM 12
#  define EACCES 13
#  define EFAULT 14
#  define EBUSY 16
#  define EEXIST 17
#  define EXDEV 18
#  define ENODEV 19
#  define ENOTDIR 20
#  define EISDIR 21
#  define EINVAL 22
#  define ENFILE 23
#  define EMFILE 24
#  define ENOTTY 25
#  define ETXTBSY 26
#  define EFBIG 27
#  define ENOSPC 28
#  define ESPIPE 29
#  define EROFS 30
#  define EMLINK 31
#  define EPIPE 32
#  define EAGAIN 35
#  define ENAMETOOLONG 63
#  define ENOLCK 77
#  define ENOSYS 78
#  define ENOTEMPTY 66
# endif /* (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 1) || _DARWIN_C_SOURCE */

# if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || defined(_DARWIN_C_SOURCE)
#  define ENOTBLK 15
#  define EWOULDBLOCK EAGAIN
#  define EINPROGRESS 36
#  define EALREADY 37
#  define ENOTSOCK 38
#  define EDESTADDRREQ 39
#  define EMSGSIZE 40
#  define EPROTOTYPE 41
#  define ENOPROTOOPT 42
#  define EPROTONOSUPPORT 43
#  define ESOCKTNOSUPPORT 44
#  define ENOTSUP 45
#  define EPFNOSUPPORT 46
#  define EAFNOSUPPORT 47
#  define EADDRINUSE 48
#  define EADDRNOTAVAIL 49
#  define ENETDOWN 50
#  define ENETUNREACH 51
#  define ENETRESET 52
#  define ECONNABORTED 53
#  define ECONNRESET 54
#  define ENOBUFS 55
#  define EISCONN 56
#  define ENOTCONN 57
#  define ESHUTDOWN 58
#  define ETOOMANYREFS 59
#  define ETIMEDOUT 60
#  define ECONNREFUSED 61
#  define ELOOP 62
#  define ESTALE 70
#  define EREMOTE 71
#  define EOVERFLOW 84
#  define ECANCELED 89
#  define EIDRM 90
#  define ENOMSG 91
#  define EBADMSG 94
#  define EMULTIHOP 95
#  define ENOLINK 97
#  define ENOSR 98
#  define EOPNOTSUPP 102
#  define ENODATA 96
#  define ENOSTR 99
#  define EPROTO 100
#  define ETIME 101
# endif /* (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112) || _DARWIN_C_SOURCE */

# if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L) || defined(_DARWIN_C_SOURCE)
#  define ENOTRECOVERABLE 104
#  define EOWNERDEAD 105
# endif /* (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200809) || _DARWIN_C_SOURCE */

# if defined(_DARWIN_C_SOURCE)
#  define EPROCLIM 67
#  define EUSERS 68
#  define EDQUOT 69
#  define EBADRPC 72
#  define ERPCMISMATCH 73
#  define EPROGUNAVAIL 74
#  define EPROGMISMATCH 75
#  define EPROCUNAVAIL 76
#  define EFTYPE 79
#  define EAUTH 80
#  define ENEEDAUTH 81
#  define EPWROFF 82
#  define EDEVERR 83
#  define EBADEXEC 85
#  define EBADARCH 86
#  define ESHLIBVERS 87
#  define EBADMACHO 88
#  define ENOATTR 93
#  define ENOPOLICY 103
#  define EQFULL 106
# endif /* _DARWIN_C_SOURCE */
#endif /* !_ANSI */

#endif /* !ERRNO_H */
