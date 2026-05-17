#include <ctype.h>
#include <locale.h>

int tolower(int c) {
    return (c > 255 || c < 0) ? c : getmap(lower, _CurrentRuneLocale, c);
}
