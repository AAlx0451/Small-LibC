#include <locale.h>
#include <wctype.h>

int iswalpha(wint_t wc) { 
    return rl_isalpha(_CurrentRuneLocale, (int)wc);
}
