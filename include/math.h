#ifndef MATH_H
#define MATH_H

#define NAN __builtin_nanf("0x7fc00000")
#define HUGE_VAL __builtin_huge_val()

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

double fabs(double x);
double floor(double x);
long double floorl(long double x);
double ceil(double x);
double sqrt(double x);
double fmod(double x, double y);
double exp(double x);
double log(double x);
double pow(double x, double y);
double frexp(double value, int *exp);
double ldexp(double x, int exp);
double modf(double value, double *iptr);
double log10(double x);
double sin(double x);
double cos(double x);
double tan(double x);
double atan2(double y, double x);
double atan(double x);
double asin(double x);
double acos(double x);
double sinh(double x);
double cosh(double x);
double tanh(double x);

#endif
