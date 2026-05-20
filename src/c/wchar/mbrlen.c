#include <wchar.h>

size_t mbrlen(const char *s, size_t n, mbstate_t *ps)
{
    static mbstate_t internal_state;
    if (!ps)
        ps = &internal_state;
    return mbrtowc(NULL, s, n, ps);
}
