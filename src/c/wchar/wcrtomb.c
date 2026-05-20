#include <errno.h>
#include <stdint.h>
#include <wchar.h>

size_t wcrtomb(char *s, wchar_t wc, __unused mbstate_t *ps)
{
    if (!s)
        return 1;
    uint32_t val = (uint32_t)wc;

    if (val < 0x80) {
        s[0] = (char)val;
        return 1;
    } else if (val < 0x800) {
        s[0] = (char)(0xC0 | (val >> 6));
        s[1] = (char)(0x80 | (val & 0x3F));
        return 2;
    } else if (val < 0x10000) {
        if (val >= 0xD800 && val <= 0xDFFF) {
            errno = EILSEQ;
            return (size_t)-1;
        }
        s[0] = (char)(0xE0 | (val >> 12));
        s[1] = (char)(0x80 | ((val >> 6) & 0x3F));
        s[2] = (char)(0x80 | (val & 0x3F));
        return 3;
    } else if (val <= 0x10FFFF) {
        s[0] = (char)(0xF0 | (val >> 18));
        s[1] = (char)(0x80 | ((val >> 12) & 0x3F));
        s[2] = (char)(0x80 | ((val >> 6) & 0x3F));
        s[3] = (char)(0x80 | (val & 0x3F));
        return 4;
    } else {
        errno = EILSEQ;
        return (size_t)-1;
    }
}
