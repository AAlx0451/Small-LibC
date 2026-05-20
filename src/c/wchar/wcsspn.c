#include <wchar.h>

size_t wcsspn(const wchar_t *s, const wchar_t *accept)
{
    size_t count = 0;
    const wchar_t *p;
    int found;
    while (*s != L'\0') {
        found = 0;
        p = accept;
        while (*p != L'\0') {
            if (*s == *p) {
                found = 1;
                break;
            }
            p++;
        }
        if (!found) {
            break;
        }
        s++;
        count++;
    }
    return count;
}
