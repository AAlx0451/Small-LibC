#ifndef TERMIOS_H
#define TERMIOS_H

#include <features.h>

#if !defined(_ANSI) && defined(_POSIX_C_SOURCE)

#include <sys/types.h>

typedef unsigned long tcflag_t;
typedef unsigned char cc_t;
typedef unsigned long speed_t;

#define NCCS 20

struct termios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t c_cc[NCCS];
#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500) || defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE)
    speed_t c_ispeed;
    speed_t c_ospeed;
#else
    speed_t __p[2];
#endif /* (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 500) || _GNU_SOURCE || _DARWIN_C_SOURCE */
};

#define VINTR    0
#define VQUIT    1
#define VERASE   2
#define VKILL    3
#define VEOF     4
#define VTIME    5
#define VMIN     6
#define VSTART   8
#define VSTOP    9
#define VSUSP    10
#define VEOL     11

#if defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE)
#define VSWTC    7
#define VREPRINT 12
#define VDISCARD 13
#define VWERASE  14
#define VLNEXT   15
#define VEOL2    16
#endif /* _GNU_SOURCE || _DARWIN_C_SOURCE */

#define IGNBRK  0x00000001
#define BRKINT  0x00000002
#define IGNPAR  0x00000004
#define PARMRK  0x00000008
#define INPCK   0x00000010
#define ISTRIP  0x00000020
#define INLCR   0x00000040
#define IGNCR   0x00000080
#define ICRNL   0x00000100
#define IXON    0x00000200
#define IXOFF   0x00000400

#if (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500) || defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE)
#define IXANY   0x00000800
#endif /* (_XOPEN_SOURCE && _XOPEN_SOURCE >= 500) || _GNU_SOURCE || _DARWIN_C_SOURCE */

#if defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE)
#define IMAXBEL 0x00002000
#endif /* _GNU_SOURCE || _DARWIN_C_SOURCE */

#define OPOST   0x00000001
#define ONLCR   0x00000002

#if defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE)
#define OXTABS  0x00000004
#define ONOEOT  0x00000008
#endif /* _GNU_SOURCE || _DARWIN_C_SOURCE */

#define CSIZE   0x00000300
#define CS5     0x00000000
#define CS6     0x00000100
#define CS7     0x00000200
#define CS8     0x00000300
#define CSTOPB  0x00000400
#define CREAD   0x00000800
#define PARENB  0x00001000
#define PARODD  0x00002000
#define HUPCL   0x00004000
#define CLOCAL  0x00008000

#if defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE)
#define ECHOKE  0x00000001
#endif /* _GNU_SOURCE || _DARWIN_C_SOURCE */

#define ECHOE   0x00000002
#define ECHOK   0x00000004
#define ECHO    0x00000008
#define ECHONL  0x00000010

#if defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE)
#define ECHOPRT 0x00000020
#define ECHOCTL 0x00000040
#endif /* _GNU_SOURCE || _DARWIN_C_SOURCE */

#define ISIG    0x00000080
#define ICANON  0x00000100
#define IEXTEN  0x00000400
#define TOSTOP  0x00400000
#define NOFLSH  0x80000000

#define TCSANOW   0
#define TCSADRAIN 1
#define TCSAFLUSH 2

#define TCIFLUSH  1
#define TCOFLUSH  2
#define TCIOFLUSH 3

#define TCOOFF 1
#define TCOON  2
#define TCIOFF 3
#define TCION  4

#define B0      0
#define B50     50
#define B75     75
#define B110    110
#define B134    134
#define B150    150
#define B200    200
#define B300    300
#define B600    600
#define B1200   1200
#define B1800   1800
#define B2400   2400
#define B4800   4800
#define B9600   9600
#define B19200  19200
#define B38400  38400

speed_t cfgetispeed(const struct termios *t);
speed_t cfgetospeed(const struct termios *t);
int cfsetispeed(struct termios *t, speed_t speed);
int cfsetospeed(struct termios *t, speed_t speed);
int tcdrain(int fd);
int tcflow(int fd, int action);
int tcflush(int fd, int queue_selector);
int tcgetattr(int fd, struct termios *t);
int tcsendbreak(int fd, int duration);
int tcsetattr(int fd, int optional_actions, const struct termios *t);

#endif /* !_ANSI && _POSIX_C_SOURCE */

#endif /* !TERMIOS_H */
