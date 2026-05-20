#include <wchar.h>

size_t wcscspn(const wchar_t *s, const wchar_t *reject)
{
    size_t count = 0;
    const wchar_t *p;
    while (*s != L'\0') {
        p = reject;
        while (*p != L'\0') {
            if (*s == *p) {
                return count;
            }
            p++;
        }
        s++;
        count++;
    }
    return count;
}
