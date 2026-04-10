#include <locale.h>
#include <wctype.h>

int iswlower(wint_t wc) { 
    return rl_islower(_CurrentRuneLocale, (int)wc);
}
