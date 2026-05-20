/* This file has been put into the public domain by its author. */

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

int _Signbitf(float f)
{
    uint32_t bits;
    memcpy(&bits, &f, sizeof(float));
    return (bits >> 31) & 1;
}
