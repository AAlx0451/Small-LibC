#include <locale.h>
#include <wctype.h>

int iswspace(wint_t wc) { 
    return rl_isspace(_CurrentRuneLocale, (int)wc);
}
