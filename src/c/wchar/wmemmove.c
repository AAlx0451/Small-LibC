#include <wchar.h>

wchar_t *wmemmove(wchar_t *dest, const wchar_t *src, size_t n)
{
    wchar_t *d = dest;
    const wchar_t *s = src;
    if (d < s) {
        while (n > 0) {
            *d = *s;
            d++;
            s++;
            n--;
        }
    } else if (d > s) {
        d += n;
        s += n;
        while (n > 0) {
            d--;
            s--;
            *d = *s;
            n--;
        }
    }
    return dest;
}
