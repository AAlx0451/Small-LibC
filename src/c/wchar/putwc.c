#include <stdio.h>
#include <wchar.h>

wint_t putwc(wchar_t c, FILE *stream) {
    return fputwc(c, stream);
}
