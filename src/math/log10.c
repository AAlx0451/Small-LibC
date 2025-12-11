#include <math.h>

/* ln(10) */
#define M_LN10 2.30258509299404568402

double log10(double x) {
    /* Relies on existing C89 log() */
    return log(x) / M_LN10;
}
