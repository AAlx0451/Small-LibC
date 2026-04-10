#include <locale.h>
#include <wctype.h>

int iswblank(wint_t wc) { 
    return rl_isblank(_CurrentRuneLocale, (int)wc);
}
