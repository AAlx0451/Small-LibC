#include <wchar.h>

double wcstod(const wchar_t *restrict nptr, wchar_t **restrict endptr) {
    return (double)wcstold(nptr, endptr);
}
