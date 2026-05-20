#include <wchar.h>

wchar_t *wcstok(wchar_t *str, const wchar_t *delim, wchar_t **ptr)
{
    wchar_t *end;
    const wchar_t *d;
    int is_delim;
    if (str == NULL) {
        str = *ptr;
    }
    while (*str != L'\0') {
        is_delim = 0;
        d = delim;
        while (*d != L'\0') {
            if (*str == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        if (!is_delim)
            break;
        str++;
    }
    if (*str == L'\0') {
        *ptr = str;
        return NULL;
    }
    end = str;
    while (*end != L'\0') {
        is_delim = 0;
        d = delim;
        while (*d != L'\0') {
            if (*end == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        if (is_delim)
            break;
        end++;
    }
    if (*end == L'\0') {
        *ptr = end;
    } else {
        *end = L'\0';
        *ptr = end + 1;
    }
    return str;
}
