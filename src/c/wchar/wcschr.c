#include <wchar.h>

wchar_t *wcschr(const wchar_t *s, wchar_t c) {
    while(*s != L'\0') {
        if(*s == c) {
            return __deconst(wchar_t *, s);
        }
        s++;
    }
    if(c == L'\0') {
        return __deconst(wchar_t *, s);
    }

    return NULL;
}
