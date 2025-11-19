#include <stdlib.h>

int mblen(const char *s, size_t n) {
    unsigned char c;
    if (s == NULL) {
        return 0;
    }

    if (n == 0) {
        return -1;
    }

    c = *s;

    if (c == '\0') {
        return 0;
    }

    if ((c & 0x80) == 0) {
        return 1;
    }

    if ((c & 0xE0) == 0xC0) {
        if (n < 2 || (s[1] & 0xC0) != 0x80) return -1;
        return 2;
    }

    if ((c & 0xF0) == 0xE0) {
        if (n < 3 || (s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80) return -1;
        return 3;
    }

    if ((c & 0xF8) == 0xF0) {
        if (n < 4 || (s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80 || (s[3] & 0xC0) != 0x80) return -1;
        return 4;
    }

    return -1;
}
