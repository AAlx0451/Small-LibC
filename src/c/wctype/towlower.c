#include <locale.h>
#include <wctype.h>

wint_t towlower(wint_t wc) {
    return (wint_t)rl_tolower(_CurrentRuneLocale, (int)wc);
}
