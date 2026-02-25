#include <stdlib.h>
#pragma clang diagnostic ignored "-Wc++-keyword"

int wctomb(char *s, wchar_t wc) {
    if(s == NULL) {
        return 0;
    }

    if(wc < 0x80) {
        *s = (char)wc;
        return 1;
    }
    if(wc < 0x800) {
        s[0] = (char)0xC0 | ((char)(wc >> 6));
        s[1] = (char)0x80 | ((char)(wc & 0x3F));
        return 2;
    }
    if(wc < 0x10000) {
        s[0] = (char)0xE0 | ((char)(wc >> 12));
        s[1] = (char)0x80 | ((char)((wc >> 6) & 0x3F));
        s[2] = (char)0x80 | ((char)(wc & 0x3F));
        return 3;
    }
    if(wc <= 0x10FFFF) {
        s[0] = (char)0xF0 | ((char)(wc >> 18));
        s[1] = (char)0x80 | ((char)((wc >> 12) & 0x3F));
        s[2] = (char)0x80 | ((char)((wc >> 6) & 0x3F));
        s[3] = (char)0x80 | ((char)(wc & 0x3F));
        return 4;
    }

    return -1;
}
