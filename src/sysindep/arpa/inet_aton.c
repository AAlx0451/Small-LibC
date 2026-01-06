#include <netinet/in.h>

int inet_aton(const char *cp, struct in_addr *inp) {
    uint32_t res = 0;
    uint32_t part = 0;
    int dots = 0;
    char c;

    if (!cp || !inp) {
        return 0;
    }

    while ((c = *cp++)) {
        if (c >= '0' && c <= '9') {
            part = part * 10 + (c - '0');
            if (part > 255) {
                return 0;
            }
        } else if (c == '.') {
            if (dots >= 3) {
                return 0;
            }
            res = (res << 8) | part;
            part = 0;
            dots++;
        } else {
            return 0;
        }
    }

    if (dots != 3) {
        return 0;
    }

    res = (res << 8) | part;
    inp->s_addr = res;
    return 1;
}
