#include <stdlib.h>
#include <wchar.h>

int wctomb(char *s, wchar_t wc) {
    if (s == NULL) {
        return 0;
    }

    if (wc < 0x80) {
        *s = wc;
        return 1;
    }
    if (wc < 0x800) {
        s[0] = 0xC0 | (wc >> 6);
        s[1] = 0x80 | (wc & 0x3F);
        return 2;
    }
    if (wc < 0x10000) {
        s[0] = 0xE0 | (wc >> 12);
        s[1] = 0x80 | ((wc >> 6) & 0x3F);
        s[2] = 0x80 | (wc & 0x3F);
        return 3;
    }
    if (wc <= 0x10FFFF) {
        s[0] = 0xF0 | (wc >> 18);
        s[1] = 0x80 | ((wc >> 12) & 0x3F);
        s[2] = 0x80 | ((wc >> 6) & 0x3F);
        s[3] = 0x80 | (wc & 0x3F);
        return 4;
    }
    
    return -1;
}
