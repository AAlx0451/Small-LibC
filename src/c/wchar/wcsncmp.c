#include <wchar.h>

int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
    if (n == 0)
        return 0;
    while (n > 1 && *s1 != L'\0' && *s1 == *s2) {
        s1++;
        s2++;
        n--;
    }
    if (*s1 < *s2)
        return -1;
    if (*s1 > *s2)
        return 1;
    return 0;
}
