#include <wchar.h>

wchar_t *wcspbrk(const wchar_t *s, const wchar_t *accept) {
    const wchar_t *p;
    while(*s != L'\0') {
        p = accept;
        while(*p != L'\0') {
            if(*s == *p) {
                return __deconst(wchar_t *, s);
            }
            p++;
        }
        s++;
    }
    return NULL;
}
