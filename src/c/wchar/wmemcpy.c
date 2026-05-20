#include <wchar.h>

wchar_t *wmemcpy(wchar_t *dest, const wchar_t *src, size_t n)
{
    wchar_t *ptr = dest;
    while (n > 0) {
        *ptr = *src;
        ptr++;
        src++;
        n--;
    }
    return dest;
}
