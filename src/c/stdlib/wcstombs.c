#include <stdlib.h>

size_t wcstombs(char *s, const wchar_t *pwcs, size_t n) {
    size_t bytes_written = 0;
    int len;

    if (s == NULL) {
        while (*pwcs) {
            char temp[MB_CUR_MAX * 4];
            len = wctomb(temp, *pwcs++);
            if (len == -1) {
                return (size_t)-1;
            }
            bytes_written += len;
        }
        return bytes_written;
    }
    
    while (bytes_written < n && *pwcs) {
        char temp_buf[MB_CUR_MAX * 4];
        len = wctomb(temp_buf, *pwcs);

        if (len == -1) {
            return (size_t)-1;
        }

        if (bytes_written + len > n) {
            break;
        }

        {
            int i;
            for (i = 0; i < len; ++i) {
                s[bytes_written + i] = temp_buf[i];
            }
        }
        
        bytes_written += len;
        pwcs++;
    }

    if (bytes_written < n) {
        s[bytes_written] = '\0';
    }

    return bytes_written;
}
