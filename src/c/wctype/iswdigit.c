#include <locale.h>
#include <wctype.h>

int iswdigit(wint_t wc) { return rl_isdigit(_CurrentRuneLocale, (int)wc); }
