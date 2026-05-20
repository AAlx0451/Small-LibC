#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

int vswprintf(wchar_t *restrict ws, size_t n, const wchar_t *restrict format, va_list ap)
{
    FILE f;
    int ret;

    /* If n is 0 return -1 */
    if (n == 0) {
        return -1;
    }

    /* zero FILE */
    memset(&f, 0, sizeof(FILE));

    f._flags = __S_WR | __S_STR;
    f._fd = -1;
    f._lock = 0;
    f._next = NULL;

    f._ptr = (unsigned char *)ws;
    f._base = (unsigned char *)ws;

    /* count in wide chars */
    f._cnt = n - 1;
    f._bsize = n - 1;

    ret = vfwprintf(&f, format, ap);

    /* NULL terminate string */
    if (f._ptr < f._base + (n * sizeof(wchar_t))) {
        wchar_t zero = L'\0';
        memcpy(f._ptr, &zero, sizeof(zero));
    } else {
        ws[n - 1] = L'\0';
    }

    __stdio_remove_file(&f);

    /* return -1 if truncated */
    if (ret >= (int)n) {
        return -1;
    }

    return ret;
}
