#include <wchar.h>

size_t wcslen(const wchar_t *s)
{
    size_t len = 0;
    while (*s != L'\0') {
        len++;
        s++;
    }
    return len;
}
