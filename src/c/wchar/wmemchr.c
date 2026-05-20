#include <wchar.h>

wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n)
{
    while (n > 0) {
        if (*s == c) {
            return __deconst(wchar_t *, s);
        }
        s++;
        n--;
    }

    return NULL;
}
