#include <stdint.h>
#include <stdlib.h>

int mbtowc(wchar_t *pwc, const char *s, size_t n) {
    if(s == NULL) {
        return 0;
    }
    if(n == 0) {
        return -1;
    }

    unsigned char c = (unsigned char)(uintptr_t)*s;
    wchar_t res = 0;
    int len = 0;

    if(c == '\0') {
        if(pwc)
            *pwc = 0;
        return 0;
    }

    if(c < 0x80) {
        len = 1;
        res = c;
    } else if((c & 0xE0) == 0xC0) {
        len = 2;
        res = c & 0x1F;
    } else if((c & 0xF0) == 0xE0) {
        len = 3;
        res = c & 0x0F;
    } else if((c & 0xF8) == 0xF0) {
        len = 4;
        res = c & 0x07;
    } else {
        return -1;
    }

    if(n < (size_t)len) {
        return -1;
    }

    for(int i = 1; i < len; ++i) {
        c = (unsigned char)(uintptr_t)s[i];
        if((c & 0xC0) != 0x80) {
            return -1;
        }
        res = (res << 6) | (c & 0x3F);
    }

    if(pwc) {
        *pwc = res;
    }

    return len;
}
