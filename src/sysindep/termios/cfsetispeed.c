#include <termios.h>

int cfsetispeed(struct termios *t, speed_t speed) {
    t->c_ispeed = speed;
    return 0;
}
