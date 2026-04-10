#include <wctype.h>
#include <locale.h>

wint_t towupper(wint_t wc) {
    return (wint_t)rl_toupper(_CurrentRuneLocale, (int)wc);
}
