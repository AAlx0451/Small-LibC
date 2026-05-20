#include <wchar.h>

wchar_t *wcscpy(wchar_t *dest, const wchar_t *src)
{
    wchar_t *ptr = dest;
    while ((*ptr = *src) != L'\0') {
        ptr++;
        src++;
    }
    return dest;
}
