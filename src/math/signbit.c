/* This file has been put into the public domain by its author. */

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

int _Signbit(double d) {
    uint64_t bits;
    memcpy(&bits, &d, sizeof(double));
    return (bits >> 63) & 1;
}
