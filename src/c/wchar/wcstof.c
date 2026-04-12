#include <wchar.h>

float wcstof(const wchar_t *restrict nptr, wchar_t **restrict endptr) {
    return (float)wcstod(nptr, endptr);
}
