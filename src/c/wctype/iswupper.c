#include <locale.h>
#include <wctype.h>

int iswupper(wint_t wc) { 
    return rl_isupper(_CurrentRuneLocale, (int)wc);
}
