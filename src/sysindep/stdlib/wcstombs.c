#include <stdlib.h>

size_t wcstombs(char *dest, const wchar_t *src, size_t n) {
    size_t bytes_written = 0;
    char temp[MB_CUR_MAX];

    while(1) {
        wchar_t wc = *src++;

        if(wc == L'\0') {
            if(dest && bytes_written < n) {
                dest[bytes_written] = '\0';
            }
            return bytes_written;
        }

        int len = wctomb(temp, wc);
        if(len == -1) {
            return (size_t)-1;
        }

        if(dest) {
            if(bytes_written + len > n) {
                return bytes_written;
            }
            for(int i = 0; i < len; ++i) {
                dest[bytes_written + i] = temp[i];
            }
        }
        bytes_written += len;
    }

    return bytes_written;
}
