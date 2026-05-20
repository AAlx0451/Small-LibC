#include <wchar.h>

size_t wcsxfrm(wchar_t *dest, const wchar_t *src, size_t n)
{
    size_t len = wcslen(src);
    if (n > len) {
        wcscpy(dest, src);
    }
    return len;
}
