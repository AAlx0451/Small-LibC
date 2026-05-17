#include <wchar.h>

wchar_t *wcsrchr(const wchar_t *s, wchar_t c) {
    const wchar_t *last = NULL;
    while(*s != L'\0') {
        if(*s == c) {
            last = s;
        }
        s++;
    }
    if(c == L'\0') {
        return __deconst(wchar_t *, s);
    }
    return __deconst(wchar_t *, last);
}
