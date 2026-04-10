#include <locale.h>
#include <wctype.h>

int iswalnum(wint_t wc) { 
    return rl_isalnum(_CurrentRuneLocale, (int)wc);
}
