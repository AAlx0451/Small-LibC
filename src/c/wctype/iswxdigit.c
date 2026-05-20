#include <locale.h>
#include <wctype.h>

int iswxdigit(wint_t wc) { return rl_isxdigit(_CurrentRuneLocale, (int)wc); }
