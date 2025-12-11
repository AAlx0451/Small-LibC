#ifndef MATH_H
#define MATH_H

#define NAN __builtin_nanf("0x7fc00000")
#define HUGE_VAL __builtin_huge_val()
#define INFINITY __builtin_huge_valf()

#include <stdint.h>

#define isfinite(x) \
	((((union { double d; uint64_t u; }){.d = (x)}.u >> 52) & 0x7FF) != 0x7FF)

#define isnan(x) \
	(((((union { double d; uint64_t u; }){.d = (x)}.u >> 52) & 0x7FF) == 0x7FF) && \
	((((union { double d; uint64_t u; }){.d = (x)}.u & 0xFFFFFFFFFFFFFULL) != 0)))

#define isinf(x) \
	(((((union { double d; uint64_t u; }){.d = (x)}.u >> 52) & 0x7FF) == 0x7FF) && \
	 ((((union { double d; uint64_t u; }){.d = (x)}.u & 0xFFFFFFFFFFFFFULL) == 0)))

#define signbit(x) (__builtin_signbit(x))

double acos(double x);
double asin(double x);
double atan(double x);
double atan2(double y, double x);
double ceil(double x);
double copysign(double x, double y);
double cos(double x);
double exp(double x);
double fabs(double x);
double fdim(double x, double y);
double floor(double x);
double fmax(double x, double y);
double fmin(double x, double y);
double fmod(double x, double y);
double frexp(double x, int *exp);
double j0(double x);
double j1(double x);
double jn(int n, double x);
double ldexp(double x, int exp);
double log(double x);
double log10(double x);
double log2(double x);
long   lround(double x);
double modf(double x, double *iptr);
double pow(double x, double y);
double remainder(double x, double y);
double round(double x);
double sin(double x);
double sqrt(double x);
double tan(double x);
double trunc(double x);

#endif
