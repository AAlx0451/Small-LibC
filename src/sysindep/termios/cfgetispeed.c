#include <termios.h>

speed_t cfgetispeed(const struct termios *t) {
    return t->c_ispeed;
}
