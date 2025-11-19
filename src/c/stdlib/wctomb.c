#include <stdlib.h>

int wctomb(char *s, wchar_t wchar) {
    if (s == NULL) {
        return 0;
    }

    if (wchar < 0x80) {
        s[0] = (char)wchar;
        return 1;
    } else if (wchar < 0x800) {
        s[0] = 0xC0 | (wchar >> 6);
        s[1] = 0x80 | (wchar & 0x3F);
        return 2;
    } else if (wchar < 0x10000) {
        s[0] = 0xE0 | (wchar >> 12);
        s[1] = 0x80 | ((wchar >> 6) & 0x3F);
        s[2] = 0x80 | (wchar & 0x3F);
        return 3;
    } else if (wchar < 0x110000) {
        s[0] = 0xF0 | (wchar >> 18);
        s[1] = 0x80 | ((wchar >> 12) & 0x3F);
        s[2] = 0x80 | ((wchar >> 6) & 0x3F);
        s[3] = 0x80 | (wchar & 0x3F);
        return 4;
    }

    return -1;
}
