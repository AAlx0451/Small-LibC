#include <wchar.h>

wchar_t *wcsncat(wchar_t *dest, const wchar_t *src, size_t n)
{
    wchar_t *ptr = dest;
    while (*ptr != L'\0') {
        ptr++;
    }
    while (n > 0 && *src != L'\0') {
        *ptr = *src;
        ptr++;
        src++;
        n--;
    }
    *ptr = L'\0';
    return dest;
}
