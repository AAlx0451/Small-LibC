#include <locale.h>
#include <wctype.h>

int iswcntrl(wint_t wc) { return rl_iscntrl(_CurrentRuneLocale, (int)wc); }
