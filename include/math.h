
#ifndef MATH_H
#define MATH_H

#include <features.h>

#if defined(__GNUC__) || defined(__clang__)
#  define HUGE_VAL  __builtin_huge_val()
#else
#  define HUGE_VAL  (1.0/0.0)
#endif /* __GNUC__ || __clang__ */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600))

#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__)
# define NAN (double)(__builtin_nanf(""))
# define INFINITY __builtin_inf()
# define isfinite(x) __builtin_isfinite(x)
# define isnan(x) __builtin_isnan(x)
# define isinf(x) __builtin_isinf(x)
# define signbit(x) __builtin_signbit(x)
#else
#warning C99 macros not supported.
#endif /* __GNUC__ || __clang__ */

double log2(double x);
double trunc(double x);
double round(double x);
double copysign(double x, double y);

#endif /* #if !_ANSI && (_DARWIN_C_SOURCE || (__STDC_VERSION__ && __STDC_VERSION__ >= 199901L) || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600)) */

double acos(double x);
double asin(double x);
double atan(double x);
double atan2(double y, double x);
double ceil(double x);
double cos(double x);
double exp(double x);
double fabs(double x);
double floor(double x);
double fmod(double x, double y);
double frexp(double x, int *exp);
double ldexp(double x, int exp);
double log(double x);
double log10(double x);
double modf(double x, double *iptr);
double pow(double x, double y);
double sin(double x);
double sqrt(double x);
double tan(double x);
double sinh(double x);
double cosh(double x);
double tanh(double x);

#endif /* !MATH_H */
