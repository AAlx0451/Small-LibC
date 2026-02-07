#ifndef TERMIO_H
#define TERMIO_H

#include <termios.h>

#define NCC 8

struct termio {
  unsigned short c_iflag;
  unsigned short c_oflag;
  unsigned short c_cflag;
  unsigned short c_lflag;
  char           c_line;
  unsigned char  c_cc[NCC];
};

#define IUCLC   0001000

#define OLCUC   0000002
#define OCRNL   0000010
#define ONOCR   0000020
#define ONLRET  0000040
#define OFILL   0000100
#define OFDEL   0000200
#define NLDLY   0000400
#define NL0     0000000
#define NL1     0000400
#define CRDLY   0003000
#define CR0     0000000
#define CR1     0001000
#define CR2     0002000
#define CR3     0003000
#define TABDLY  0014000
#define TAB0    0000000
#define TAB1    0004000
#define TAB2    0100000
#define TAB3    0014000
#define BSDLY   0020000
#define BS0     0000000
#define BS1     0020000
#define VTDLY   0040000
#define VT0     0000000
#define VT1     0040000
#define FFDLY   0100000
#define FF0     0000000
#define FF1     0100000

#define CBAUD   0000017
#define EXTA    B19200
#define EXTB    B38400

#define XCASE   0000004

#define IOCTLSORT ('t' << 8)
#define TCGETA    (IOCTLSORT|1)
#define TCSETA    (IOCTLSORT|2)
#define TCSETAW   (IOCTLSORT|3)
#define TCSETAF   (IOCTLSORT|4)
#define TCSBRK    (IOCTLSORT|5)
#define TCXONC    (IOCTLSORT|6)
#define TCFLSH    (IOCTLSORT|7)

#endif
