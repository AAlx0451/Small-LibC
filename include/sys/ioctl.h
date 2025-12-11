/*
 * Copyright (c) 2000 Apple Computer, Inc. All rights reserved.
 */

/* Copyright (c) 1995 NeXT Computer, Inc. All Rights Reserved */

/*
 * Copyright (c) 1982, 1986, 1990, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 */

 /* 
  * (c) UNIX System Laboratories, Inc.
  */

#ifndef	_SYS_IOCTL_H_
#define	_SYS_IOCTL_H_

#include <sys/ttycom.h>

/*
 * Pun for SunOS prior to 3.2.  SunOS 3.2 and later support TIOCGWINSZ
 * and TIOCSWINSZ (yes, even 3.2-3.5, the fact that it wasn't documented
 * nonwithstanding).
 */
struct ttysize {
	unsigned short	ts_lines;
	unsigned short	ts_cols;
	unsigned short	ts_xxx;
	unsigned short	ts_yyy;
};
#define	TIOCGSIZE	TIOCGWINSZ
#define	TIOCSSIZE	TIOCSWINSZ

int	ioctl(int, unsigned long, ...);
#endif /* !_SYS_IOCTL_H_ */
