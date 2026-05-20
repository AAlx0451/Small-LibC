#include <wchar.h>

wchar_t *wcscat(wchar_t *dest, const wchar_t *src)
{
    wchar_t *ptr = dest;
    while (*ptr != L'\0') {
        ptr++;
    }
    while (*src != L'\0') {
        *ptr = *src;
        ptr++;
        src++;
    }
    *ptr = L'\0';
    return dest;
}
