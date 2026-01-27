#ifndef	SYS_FILIO_H
#define	SYS_FILIO_H

#include <sys/ioccom.h>

#define	FIOCLEX	_IO('f', 1)
#define	FIONCLEX _IO('f', 2)
#define	FIONREAD _IOR('f', 127, int)
#define	FIONBIO	_IOW('f', 126, int)
#define	FIOASYNC _IOW('f', 125, int)
#define	FIOSETOWN _IOW('f', 124, int)
#define	FIOGETOWN _IOR('f', 123, int)
#define	FIODTYPE _IOR('f', 122, int)

#endif // SYS_FILIO_H
