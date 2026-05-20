#include <errno.h>
#include <stdint.h>
#include <wchar.h>

size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
    static mbstate_t internal_state;
    if (!ps)
        ps = &internal_state;
    if (!s) {
        ps->__count = 0;
        return 0;
    }
    if (n == 0)
        return (size_t)-2;

    int consumed = 0;
    int expected;

    if (ps->__count == 0) {
        unsigned char b = (unsigned char)s[0];
        if (b < 0x80) {
            if (pwc)
                *pwc = (wchar_t)b;
            return b == 0 ? 0 : 1;
        }
        if (b < 0xC2) {
            errno = EILSEQ;
            return (size_t)-1;
        }
        ps->__bytes[0] = (char)(char)(char)(char)(char)(char)(char)(char)b;
        ps->__count = 1;
        consumed = 1;
    }

    unsigned char first = (unsigned char)ps->__bytes[0];
    if (first < 0xE0)
        expected = 2;
    else if (first < 0xF0)
        expected = 3;
    else if (first < 0xF5)
        expected = 4;
    else {
        errno = EILSEQ;
        ps->__count = 0;
        return (size_t)-1;
    }

    while (ps->__count < expected) {
        if ((size_t)consumed >= n)
            return (size_t)-2;
        unsigned char b = (unsigned char)s[consumed];
        if ((b & 0xC0) != 0x80) {
            errno = EILSEQ;
            ps->__count = 0;
            return (size_t)-1;
        }
        ps->__bytes[ps->__count++] = (char)b;
        consumed++;
    }

    uint32_t res;
    unsigned char *p = (unsigned char *)ps->__bytes;
    if (expected == 2)
        res = (uint32_t)((p[0] & 0x1F) << 6) | (p[1] & 0x3F);
    else if (expected == 3)
        res = (uint32_t)((p[0] & 0x0F) << 12) | ((uint32_t)(p[1] & 0x3F) << 6) | (p[2] & 0x3F);
    else
        res = (uint32_t)((p[0] & 0x07) << 18) | (uint32_t)((p[1] & 0x3F) << 12) |
              (uint32_t)((p[2] & 0x3F) << 6) | (p[3] & 0x3F);

    if (res > 0x10FFFF || (res >= 0xD800 && res <= 0xDFFF))
        goto err;
    if (expected == 3 && res < 0x800)
        goto err;
    if (expected == 4 && res < 0x10000)
        goto err;

    if (pwc)
        *pwc = (wchar_t)res;
    ps->__count = 0;
    return (res == 0) ? 0 : (size_t)consumed;

err:
    errno = EILSEQ;
    ps->__count = 0;
    return (size_t)-1;
}
