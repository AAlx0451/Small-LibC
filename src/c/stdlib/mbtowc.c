#include <stdlib.h>

int mbtowc(wchar_t *pwc, const char *s, size_t n) {
    const unsigned char *us = (const unsigned char *)s;
    wchar_t val;
    int len;

    if (s == NULL) {
        return 0;
    }

    if (n == 0) {
        return -1;
    }

    if (*us == '\0') {
        if (pwc) *pwc = 0;
        return 0;
    }

    if ((*us & 0x80) == 0) {
        len = 1;
        val = *us;
    } else if ((*us & 0xE0) == 0xC0) {
        len = 2;
        if (n < 2 || (us[1] & 0xC0) != 0x80) return -1;
        val = ((us[0] & 0x1F) << 6) | (us[1] & 0x3F);
    } else if ((*us & 0xF0) == 0xE0) {
        len = 3;
        if (n < 3 || (us[1] & 0xC0) != 0x80 || (us[2] & 0xC0) != 0x80) return -1;
        val = ((us[0] & 0x0F) << 12) | ((us[1] & 0x3F) << 6) | (us[2] & 0x3F);
    } else if ((*us & 0xF8) == 0xF0) {
        len = 4;
        if (n < 4 || (us[1] & 0xC0) != 0x80 || (us[2] & 0xC0) != 0x80 || (us[3] & 0xC0) != 0x80) return -1;
        val = ((us[0] & 0x07) << 18) | ((us[1] & 0x3F) << 12) | ((us[2] & 0x3F) << 6) | (us[3] & 0x3F);
    } else {
        return -1;
    }

    if (pwc) {
        *pwc = val;
    }
    
    return len;
}
