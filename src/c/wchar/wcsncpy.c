#include <wchar.h>

wchar_t *wcsncpy(wchar_t *dest, const wchar_t *src, size_t n)
{
    wchar_t *ptr = dest;
    while (n > 0 && *src != L'\0') {
        *ptr = *src;
        ptr++;
        src++;
        n--;
    }
    while (n > 0) {
        *ptr = L'\0';
        ptr++;
        n--;
    }
    return dest;
}
