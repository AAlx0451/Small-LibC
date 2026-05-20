#include <locale.h>
#include <wctype.h>

int iswpunct(wint_t wc) { return rl_ispunct(_CurrentRuneLocale, (int)wc); }
