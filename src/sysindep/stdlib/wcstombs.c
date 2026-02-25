#include <stdlib.h>
#pragma clang diagnostic ignored "-Wc++-keyword"

size_t wcstombs(char *dest, const wchar_t *src, size_t n) {
    size_t bytes_written = 0;
    char temp[MB_CUR_MAX];
    int len;

    while(1) {
        wchar_t wc = *src++;

        if(wc == L'\0') {
            if(dest && bytes_written < n) {
                dest[bytes_written] = '\0';
            }
            return bytes_written;
        }

        len = wctomb(temp, wc);
        if(len == -1) {
            return (size_t)-1;
        }

        if(dest) {
            if(bytes_written + (size_t)len > n) {
                return bytes_written;
            }
            for(unsigned i = 0; i < (unsigned long)len; ++i) {
                dest[bytes_written + i] = temp[i];
            }
        }
        bytes_written += (size_t)len;
    }
}
