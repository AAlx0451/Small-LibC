#include <wctype.h>

wint_t towctrans(wint_t wc, wctrans_t desc) {
    switch (desc) {
        case 1: return towlower(wc);
        case 2: return towupper(wc);
        default: return wc;
    }
}
