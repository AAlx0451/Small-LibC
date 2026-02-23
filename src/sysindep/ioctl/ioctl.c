#include <stdarg.h>
#include <string.h>
#include <sys/syscall.h>
#include <termio.h>
#include <termios.h>
#include <unistd.h>

static inline __attribute__((always_inline)) int termio_baud_to_speed(unsigned short cflag) {
    unsigned short baud = cflag & CBAUD;
    switch(baud) {
    case 0:
        return B0;
    case 1:
        return B50;
    case 2:
        return B75;
    case 3:
        return B110;
    case 4:
        return B134;
    case 5:
        return B150;
    case 6:
        return B200;
    case 7:
        return B300;
    case 8:
        return B600;
    case 9:
        return B1200;
    case 10:
        return B1800;
    case 11:
        return B2400;
    case 12:
        return B4800;
    case 13:
        return B9600;
    case 14:
        return EXTA;
    case 15:
        return EXTB;
    default:
        return B9600;
    }
}

static inline __attribute__((always_inline)) unsigned short speed_to_termio_baud(speed_t speed) {
    switch(speed) {
    case B0:
        return 0;
    case B50:
        return 1;
    case B75:
        return 2;
    case B110:
        return 3;
    case B134:
        return 4;
    case B150:
        return 5;
    case B200:
        return 6;
    case B300:
        return 7;
    case B600:
        return 8;
    case B1200:
        return 9;
    case B1800:
        return 10;
    case B2400:
        return 11;
    case B4800:
        return 12;
    case B9600:
        return 13;
    case B19200:
        return 14;
    case B38400:
        return 15;
    default:
        return 13;
    }
}

static inline __attribute__((always_inline)) void trans_termios_to_termio(const struct termios *src, struct termio *dst) {
    dst->c_iflag = (unsigned short)src->c_iflag;
    dst->c_lflag = (unsigned short)src->c_lflag;

    dst->c_cflag = (unsigned short)src->c_cflag & ~CBAUD;
    dst->c_cflag |= speed_to_termio_baud(src->c_ospeed);

    dst->c_oflag = (unsigned short)(src->c_oflag & 0xFFFF);
    if(src->c_oflag & OXTABS) {
        dst->c_oflag |= TAB3;
    }

    dst->c_line = 0;
    memcpy(dst->c_cc, src->c_cc, NCC);
}

static inline __attribute__((always_inline)) void trans_termio_to_termios(const struct termio *src, struct termios *dst) {
    dst->c_iflag = src->c_iflag;
    dst->c_lflag = src->c_lflag;

    dst->c_cflag = (dst->c_cflag & ~0xFFFF) | (src->c_cflag & ~CBAUD);

    speed_t speed = termio_baud_to_speed(src->c_cflag);
    dst->c_ispeed = speed;
    dst->c_ospeed = speed;

    dst->c_oflag = (dst->c_oflag & ~0xFFFF) | src->c_oflag;
    if(src->c_oflag & TAB3) {
        dst->c_oflag |= OXTABS;
        dst->c_oflag &= ~TAB3;
    }

    memcpy(dst->c_cc, src->c_cc, NCC);
}

int ioctl(int fd, unsigned long request, ...) {
    va_list ap;
    long arg;
    struct termio *tio;
    struct termios tios;
    int ret;
    int action, queue;

    va_start(ap, request);
    arg = va_arg(ap, long);
    va_end(ap);

    switch(request) {
    case TCGETA:
        ret = tcgetattr(fd, &tios);
        if(ret == 0) {
            tio = (struct termio *)arg;
            trans_termios_to_termio(&tios, tio);
        }
        return ret;

    case TCSETA:
    case TCSETAW:
    case TCSETAF:
        tio = (struct termio *)arg;
        if(tcgetattr(fd, &tios) < 0)
            return -1;

        trans_termio_to_termios(tio, &tios);

        if(request == TCSETAF)
            action = TCSAFLUSH;
        else if(request == TCSETAW)
            action = TCSADRAIN;
        else
            action = TCSANOW;

        return tcsetattr(fd, action, &tios);

    case TCSBRK:
        return tcsendbreak(fd, 0);

    case TCXONC:
        switch((int)arg) {
        case 0:
            action = TCOOFF;
            break;
        case 1:
            action = TCOON;
            break;
        case 2:
            action = TCIOFF;
            break;
        case 3:
            action = TCION;
            break;
        default:
            return -1;
        }
        return tcflow(fd, action);

    case TCFLSH:
        switch((int)arg) {
        case 0:
            queue = TCIFLUSH;
            break;
        case 1:
            queue = TCOFLUSH;
            break;
        case 2:
            queue = TCIOFLUSH;
            break;
        default:
            return -1;
        }
        return tcflush(fd, queue);

    default:
        return (int)syscall(SYS_ioctl, fd, (long)request, arg);
    }
}
