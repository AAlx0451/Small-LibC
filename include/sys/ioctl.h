#ifndef	SYS_IOCTL_H
#define	SYS_IOCTL_H

#include <sys/ttycom.h>

struct ttysize {
	unsigned short	ts_lines;
	unsigned short	ts_cols;
	unsigned short	ts_xxx;
	unsigned short	ts_yyy;
};

#define TIOCGSIZE TIOCGWINSZ
#define TIOCSSIZE TIOCSWINSZ

#include <sys/ioccom.h>
#include <sys/filio.h>

int ioctl(int, unsigned long, ...);

#endif // SYS_IOCTL_H
