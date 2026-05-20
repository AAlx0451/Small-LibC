#include <wchar.h>

wchar_t *wcsstr(const wchar_t *haystack, const wchar_t *needle)
{
    const wchar_t *h;
    const wchar_t *n;
    if (*needle == L'\0') {
        return __deconst(wchar_t *, haystack);
    }

    while (*haystack != L'\0') {
        h = haystack;
        n = needle;
        while (*n != L'\0' && *h == *n) {
            h++;
            n++;
        }
        if (*n == L'\0') {
            return __deconst(wchar_t *, haystack);
        }
        haystack++;
    }

    return NULL;
}
