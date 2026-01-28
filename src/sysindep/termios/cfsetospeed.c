#include <termios.h>

int cfsetospeed(struct termios *t, speed_t speed) {
    t->c_ospeed = speed;
    return 0;
}
