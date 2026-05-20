#include <wchar.h>

int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
    while (n > 0) {
        if (*s1 != *s2) {
            if (*s1 < *s2)
                return -1;
            return 1;
        }
        s1++;
        s2++;
        n--;
    }
    return 0;
}
