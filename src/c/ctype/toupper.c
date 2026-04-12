#include <ctype.h>
#include <locale.h>

int toupper(int c) {
    return (c > 255 || c < 0) ? c : (_CurrentRuneLocale->mapupper[c]);
}
