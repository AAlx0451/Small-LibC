#include <wchar.h>

wchar_t *wmemset(wchar_t *s, wchar_t c, size_t n)
{
    wchar_t *ptr = s;
    while (n > 0) {
        *ptr = c;
        ptr++;
        n--;
    }
    return s;
}
