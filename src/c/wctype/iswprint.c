#include <locale.h>
#include <wctype.h>

int iswprint(wint_t wc) { return rl_isprint(_CurrentRuneLocale, (int)wc); }
