/* This file has been put into the public domain by its author. */

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

double nan(const char *tagp)
{
    uint64_t payload = 0;
    if (tagp && *tagp) {
        payload = strtoull(tagp, NULL, 0);
    }

    /* 0x7FF0000000000000 - exp
     * 0x0008000000000000 - mant msb
     * 0x000FFFFFFFFFFFFF - payload
     */

    uint64_t bits = 0x7FF8000000000000ULL | (payload & 0x000FFFFFFFFFFFFFULL);

    double d;
    memcpy(&d, &bits, sizeof(double));
    return d;
}
