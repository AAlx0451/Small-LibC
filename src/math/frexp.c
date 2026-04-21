/* This file has been put into the public domain by its author. */

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

double frexp(double num, int *ex) {
    uint64_t bits;
    int e;
    *ex = 0;

    switch(fpclassify(num)) {
    case FP_NAN:
    case FP_INFINITE:
    case FP_ZERO:
        return num;
    case FP_SUBNORMAL:
        /* normalize */
        while(fpclassify(num) == FP_SUBNORMAL) {
            *ex -= 30;
            num *= 1073741824.0;
        }
        break;
    }

    memcpy(&bits, &num, sizeof(double));

    /* Bias - 1 = 1022 */
    e = ((bits >> 52) & 0x7FF) - 1022;
    *ex += e;

    bits = (bits & 0x800FFFFFFFFFFFFFULL) | (0x3FEULL << 52);

    memcpy(&num, &bits, sizeof(double));
    return num;
}
