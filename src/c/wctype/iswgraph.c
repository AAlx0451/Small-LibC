#include <locale.h>
#include <wctype.h>

int iswgraph(wint_t wc) {
    return rl_isgraph(_CurrentRuneLocale, (int)wc);
}
