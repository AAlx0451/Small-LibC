
#include <errno.h>
#include <wchar.h>

size_t mbsrtowcs(wchar_t *dst, const char **src, size_t len, mbstate_t *ps)
{
    static mbstate_t internal_state;
    if (!ps)
        ps = &internal_state;
    const char *s = *src;
    size_t count = 0;

    while (!dst || count < len) {
        wchar_t wc;
        size_t r = mbrtowc(dst ? &dst[count] : &wc, s, 4, ps);

        if (r == (size_t)-1) {
            return (size_t)-1;
        }
        if (r == (size_t)-2) {
            errno = EILSEQ;
            return (size_t)-1;
        }
        if (r == 0) {
            if (dst)
                *src = NULL;
            return count;
        }
        s += r;
        count++;
    }

    if (dst)
        *src = s;
    return count;
}
