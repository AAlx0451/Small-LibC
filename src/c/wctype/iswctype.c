#include <wctype.h>

int iswctype(wint_t wc, wctype_t desc) {
    switch (desc) {
        case 1:  return iswalnum(wc);
        case 2:  return iswalpha(wc);
        case 3:  return iswblank(wc);
        case 4:  return iswcntrl(wc);
        case 5:  return iswdigit(wc);
        case 6:  return iswgraph(wc);
        case 7:  return iswlower(wc);
        case 8:  return iswprint(wc);
        case 9:  return iswpunct(wc);
        case 10: return iswspace(wc);
        case 11: return iswupper(wc);
        case 12: return iswxdigit(wc);
        default: return 0;
    }
}
