#include <stdlib.h>

size_t mbstowcs(wchar_t *pwcs, const char *s, size_t n) {
    size_t count = 0;
    int len;

    if (pwcs == NULL) {
        while (*s) {
            len = mblen(s, MB_CUR_MAX * 4);
            if (len == -1) {
                return (size_t)-1;
            }
            if (len == 0) {
                break;
            }
            s += len;
            count++;
        }
        return count;
    }
    while (count < n) {
        wchar_t wc;
        len = mbtowc(&wc, s, MB_CUR_MAX * 4);
        
        if (len == -1) {
            return (size_t)-1;
        }
        
        if (len == 0) {
            *pwcs = L'\0';
            return count;
        }

        *pwcs++ = wc;
        s += len;
        count++;
    }

    return count;
}
