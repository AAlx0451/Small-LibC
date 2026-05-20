#include <wchar.h>

size_t wcsrtombs(char *dst, const wchar_t **src, size_t len, mbstate_t *ps)
{
    static mbstate_t internal_state;
    if (!ps)
        ps = &internal_state;
    const wchar_t *ws = *src;
    size_t count = 0;
    char buf[4];

    while (1) {
        wchar_t wc = *ws;
        size_t r = wcrtomb(dst ? buf : buf, wc, ps);
        if (r == (size_t)-1)
            return (size_t)-1;

        if (dst) {
            if (count + r > len)
                break;
            for (size_t i = 0; i < r; i++)
                dst[count + i] = buf[i];
        }

        count += r;
        if (wc == L'\0') {
            if (dst)
                *src = NULL;
            return count - 1;
        }
        ws++;
        if (dst && count == len)
            break;
    }

    if (dst)
        *src = ws;
    return count;
}
