#ifndef SYS_IOCCOM_H
#define SYS_IOCCOM_H

#include <features.h>

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_XOPEN_SOURCE))

#define _IOC_NRBITS     8
#define _IOC_TYPEBITS   8
#define _IOC_SIZEBITS   13
#define _IOC_DIRBITS    3

#define _IOC_NRSHIFT    0
#define _IOC_TYPESHIFT  (_IOC_NRSHIFT + _IOC_NRBITS)
#define _IOC_SIZESHIFT  (_IOC_TYPESHIFT + _IOC_TYPEBITS)
#define _IOC_DIRSHIFT   29

#define _IOC_SIZEMASK   ((1 << _IOC_SIZEBITS) - 1)
#define _IOC_TYPEMASK   ((1 << _IOC_TYPEBITS) - 1)

#define _IOC_NONE       (1U << 29)
#define _IOC_READ       (1U << 30)
#define _IOC_WRITE      (1U << 31)

#define _IOC(dir, type, nr, size) \
	((unsigned int)(dir) | \
	(((size) & _IOC_SIZEMASK) << _IOC_SIZESHIFT) | \
	((type) << _IOC_TYPESHIFT) | \
	(nr))

#define _IO(g, n)       _IOC(_IOC_NONE, (g), (n), 0)
#define _IOR(g, n, t)   _IOC(_IOC_READ, (g), (n), sizeof(t))
#define _IOW(g, n, t)   _IOC(_IOC_WRITE, (g), (n), sizeof(t))
#define _IOWR(g, n, t)  _IOC(_IOC_READ | _IOC_WRITE, (g), (n), sizeof(t))

#define IOCPARM_MASK    _IOC_SIZEMASK
#define IOCPARM_LEN(x)  (((x) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)
#define IOCBASECMD(x)   ((x) & ~(_IOC_SIZEMASK << _IOC_SIZESHIFT))
#define IOCGROUP(x)     (((x) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)

#define IOCPARM_MAX     (_IOC_SIZEMASK + 1)
#define IOC_VOID        _IOC_NONE
#define IOC_OUT         _IOC_READ
#define IOC_IN          _IOC_WRITE
#define IOC_INOUT       (IOC_IN | IOC_OUT)
#define IOC_DIRMASK     0xe0000000

#endif /* !_ANSI && (_DARWIN_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !SYS_IOCCOM_H */
