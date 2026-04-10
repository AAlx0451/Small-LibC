#include <wctype.h>
#include <string.h>

wctrans_t wctrans(const char *property) {
    if (strcmp(property, "tolower") == 0) return 1;
    if (strcmp(property, "toupper") == 0) return 2;
    return 0;
}
