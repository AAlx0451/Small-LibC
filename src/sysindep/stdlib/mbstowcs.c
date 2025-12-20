#include <stdlib.h>
#include <wchar.h>

size_t mbstowcs(wchar_t *dest, const char *src, size_t n) {
    size_t count = 0;
    wchar_t wc;
    
    while (1) {
        if (dest && count >= n) {
            break;
        }

        int len = mbtowc(&wc, src, (size_t)-1);

        if (len == -1) {
            return (size_t)-1;
        }
        
        if (len == 0) {
            if (dest) {
                dest[count] = L'\0';
            }
            return count;
        }

        if (dest) {
            dest[count] = wc;
        }

        src += len;
        count++;
    }

    return count;
}
