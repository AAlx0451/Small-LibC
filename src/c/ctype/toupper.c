#include <ctype.h>
#include <locale.h>

int toupper(int c) { return (c > 255 || c < 0) ? c : getmap(upper, _CurrentRuneLocale, c); }
