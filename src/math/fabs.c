/* This file has been put into the public domain by its author. */

#include <math.h>
#include <stdint.h>
#include <string.h>

double fabs(double x)
{
    uint64_t bits;
    memcpy(&bits, &x, sizeof(double));

    bits &= 0x7FFFFFFFFFFFFFFFULL; /* reset sign bit */
    memcpy(&x, &bits, sizeof(double));

    return x;
}
