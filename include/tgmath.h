#ifndef _TGMATH_H
#define _TGMATH_H

// #include <complex.h> not implemented yet!
#include <math.h>

#define acos(x)                                                                                    \
    _Generic((x),                                                                                  \
        long double complex: cacosl(x),                                                            \
        double complex: cacos(x),                                                                  \
        float complex: cacosf(x),                                                                  \
        long double: acosl(x),                                                                     \
        double: acos(x),                                                                           \
        float: acosf(x),                                                                           \
        default: acos((double)x))

#define acosh(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double complex: cacoshl(x),                                                           \
        double complex: cacosh(x),                                                                 \
        float complex: cacoshf(x),                                                                 \
        long double: acoshl(x),                                                                    \
        double: acosh(x),                                                                          \
        float: acoshf(x),                                                                          \
        default: acosh((double)x))

#define asin(x)                                                                                    \
    _Generic((x),                                                                                  \
        long double complex: casinl(x),                                                            \
        double complex: casin(x),                                                                  \
        float complex: casinf(x),                                                                  \
        long double: asinl(x),                                                                     \
        double: asin(x),                                                                           \
        float: asinf(x),                                                                           \
        default: asin((double)x))

#define asinh(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double complex: casinhl(x),                                                           \
        double complex: casinh(x),                                                                 \
        float complex: casinhf(x),                                                                 \
        long double: asinhl(x),                                                                    \
        double: asinh(x),                                                                          \
        float: asinhf(x),                                                                          \
        default: asinh((double)x))

#define atan(x)                                                                                    \
    _Generic((x),                                                                                  \
        long double complex: catanl(x),                                                            \
        double complex: catan(x),                                                                  \
        float complex: catanf(x),                                                                  \
        long double: atanl(x),                                                                     \
        double: atan(x),                                                                           \
        float: atanf(x),                                                                           \
        default: atan((double)x))

#define atan2(x, y)                                                                                \
    _Generic((x) * (y),                                                                            \
        long double: atan2l(x, y),                                                                 \
        double: atan2(x, y),                                                                       \
        float: atan2f(x, y),                                                                       \
        default: atan2((double)x, (double)y))

#define atanh(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double complex: catanhl(x),                                                           \
        double complex: catanh(x),                                                                 \
        float complex: catanhf(x),                                                                 \
        long double: atanhl(x),                                                                    \
        double: atanh(x),                                                                          \
        float: atanhf(x),                                                                          \
        default: atanh((double)x))

#define carg(x)                                                                                    \
    _Generic((x),                                                                                  \
        long double complex: cargl(x),                                                             \
        double complex: carg(x),                                                                   \
        float complex: cargf(x),                                                                   \
        default: carg((double)x))

#define cbrt(x)                                                                                    \
    _Generic((x), long double: cbrtl(x), double: cbrt(x), float: cbrtf(x), default: cbrt((double)x))

#define ceil(x)                                                                                    \
    _Generic((x), long double: ceill(x), double: ceil(x), float: ceilf(x), default: ceil((double)x))

#define cimag(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double complex: cimagl(x),                                                            \
        double complex: cimag(x),                                                                  \
        float complex: cimagf(x),                                                                  \
        default: cimag((double)x))

#define conj(x)                                                                                    \
    _Generic((x),                                                                                  \
        long double complex: conjl(x),                                                             \
        double complex: conj(x),                                                                   \
        float complex: conjf(x),                                                                   \
        default: conj((double)x))

#define copysign(x, y)                                                                             \
    _Generic((x) * (y),                                                                            \
        long double: copysignl(x, y),                                                              \
        double: copysign(x, y),                                                                    \
        float: copysignf(x, y),                                                                    \
        default: copysign((double)x, (double)y))

#define cos(x)                                                                                     \
    _Generic((x),                                                                                  \
        long double complex: ccosl(x),                                                             \
        double complex: ccos(x),                                                                   \
        float complex: ccosf(x),                                                                   \
        long double: cosl(x),                                                                      \
        double: cos(x),                                                                            \
        float: cosf(x),                                                                            \
        default: cos((double)x))

#define cosh(x)                                                                                    \
    _Generic((x),                                                                                  \
        long double complex: ccoshl(x),                                                            \
        double complex: ccosh(x),                                                                  \
        float complex: ccoshf(x),                                                                  \
        long double: coshl(x),                                                                     \
        double: cosh(x),                                                                           \
        float: coshf(x),                                                                           \
        default: cosh((double)x))

#define cproj(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double complex: cprojl(x),                                                            \
        double complex: cproj(x),                                                                  \
        float complex: cprojf(x),                                                                  \
        default: cproj((double)x))

#define creal(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double complex: creall(x),                                                            \
        double complex: creal(x),                                                                  \
        float complex: crealf(x),                                                                  \
        default: creal((double)x))

#define erf(x)                                                                                     \
    _Generic((x), long double: erfl(x), double: erf(x), float: erff(x), default: erf((double)x))

#define erfc(x)                                                                                    \
    _Generic((x), long double: erfcl(x), double: erfc(x), float: erfcf(x), default: erfc((double)x))

#define exp(x)                                                                                     \
    _Generic((x),                                                                                  \
        long double complex: cexpl(x),                                                             \
        double complex: cexp(x),                                                                   \
        float complex: cexpf(x),                                                                   \
        long double: expl(x),                                                                      \
        double: exp(x),                                                                            \
        float: expf(x),                                                                            \
        default: exp((double)x))

#define exp2(x)                                                                                    \
    _Generic((x), long double: exp2l(x), double: exp2(x), float: exp2f(x), default: exp2((double)x))

#define expm1(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double: expm1l(x),                                                                    \
        double: expm1(x),                                                                          \
        float: expm1f(x),                                                                          \
        default: expm1((double)x))

#define fabs(x)                                                                                    \
    _Generic((x),                                                                                  \
        long double complex: cabsl(x),                                                             \
        double complex: cabs(x),                                                                   \
        float complex: cabsf(x),                                                                   \
        long double: fabsl(x),                                                                     \
        double: fabs(x),                                                                           \
        float: fabsf(x),                                                                           \
        default: fabs((double)x))

#define fdim(x, y)                                                                                 \
    _Generic((x) * (y),                                                                            \
        long double: fdiml(x, y),                                                                  \
        double: fdim(x, y),                                                                        \
        float: fdimf(x, y),                                                                        \
        default: fdim((double)x, (double)y))

#define floor(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double: floorl(x),                                                                    \
        double: floor(x),                                                                          \
        float: floorf(x),                                                                          \
        default: floor((double)x))

#define fma(x, y, z)                                                                               \
    _Generic((x) * (y) * (z),                                                                      \
        long double: fmal(x, y, z),                                                                \
        double: fma(x, y, z),                                                                      \
        float: fmaf(x, y, z),                                                                      \
        default: fma((double)x, (double)y, (double)z))

#define fmax(x, y)                                                                                 \
    _Generic((x) * (y),                                                                            \
        long double: fmaxl(x, y),                                                                  \
        double: fmax(x, y),                                                                        \
        float: fmaxf(x, y),                                                                        \
        default: fmax((double)x, (double)y))

#define fmin(x, y)                                                                                 \
    _Generic((x) * (y),                                                                            \
        long double: fminl(x, y),                                                                  \
        double: fmin(x, y),                                                                        \
        float: fminf(x, y),                                                                        \
        default: fmin((double)x, (double)y))

#define fmod(x, y)                                                                                 \
    _Generic((x) * (y),                                                                            \
        long double: fmodl(x, y),                                                                  \
        double: fmod(x, y),                                                                        \
        float: fmodf(x, y),                                                                        \
        default: fmod((double)x, (double)y))

#define frexp(x, y)                                                                                \
    _Generic((x),                                                                                  \
        long double: frexpl(x, y),                                                                 \
        double: frexp(x, y),                                                                       \
        float: frexpf(x, y),                                                                       \
        default: frexp((double)x, y))

#define hypot(x, y)                                                                                \
    _Generic((x) * (y),                                                                            \
        long double: hypotl(x, y),                                                                 \
        double: hypot(x, y),                                                                       \
        float: hypotf(x, y),                                                                       \
        default: hypot((double)x, (double)y))

#define ilog(x)                                                                                    \
    _Generic((x), long double: ilogl(x), double: ilog(x), float: ilogf(x), default: ilog((double)x))

#define ldexp(x, y)                                                                                \
    _Generic((x),                                                                                  \
        long double: ldexpl(x, y),                                                                 \
        double: ldexp(x, y),                                                                       \
        float: ldexpf(x, y),                                                                       \
        default: ldexp((double)x, y))

#define lgamma(x)                                                                                  \
    _Generic((x),                                                                                  \
        long double: lgammal(x),                                                                   \
        double: lgamma(x),                                                                         \
        float: lgammaf(x),                                                                         \
        default: lgamma((double)x))

#define llrint(x)                                                                                  \
    _Generic((x),                                                                                  \
        long double: llrintl(x),                                                                   \
        double: llrint(x),                                                                         \
        float: llrintf(x),                                                                         \
        default: llrint((double)x))

#define llround(x)                                                                                 \
    _Generic((x),                                                                                  \
        long double: llroundl(x),                                                                  \
        double: llround(x),                                                                        \
        float: llroundf(x),                                                                        \
        default: llround((double)x))

#define log(x)                                                                                     \
    _Generic((x),                                                                                  \
        long double complex: clogl(x),                                                             \
        double complex: clog(x),                                                                   \
        float complex: clogf(x),                                                                   \
        long double: logl(x),                                                                      \
        double: log(x),                                                                            \
        float: logf(x),                                                                            \
        default: log((double)x))

#define log10(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double: log10l(x),                                                                    \
        double: log10(x),                                                                          \
        float: log10f(x),                                                                          \
        default: log10((double)x))

#define log1p(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double: log1pl(x),                                                                    \
        double: log1p(x),                                                                          \
        float: log1pf(x),                                                                          \
        default: log1p((double)x))

#define log2(x)                                                                                    \
    _Generic((x), long double: log2l(x), double: log2(x), float: log2f(x), default: log2((double)x))

#define logb(x)                                                                                    \
    _Generic((x), long double: logbl(x), double: logb(x), float: logbf(x), default: logb((double)x))

#define lrint(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double: lrintl(x),                                                                    \
        double: lrint(x),                                                                          \
        float: lrintf(x),                                                                          \
        default: lrint((double)x))

#define lround(x)                                                                                  \
    _Generic((x),                                                                                  \
        long double: lroundl(x),                                                                   \
        double: lround(x),                                                                         \
        float: lroundf(x),                                                                         \
        default: lround((double)x))

#define nearbyint(x)                                                                               \
    _Generic((x),                                                                                  \
        long double: nearbyintl(x),                                                                \
        double: nearbyint(x),                                                                      \
        float: nearbyintf(x),                                                                      \
        default: nearbyint((double)x))

#define nextafter(x, y)                                                                            \
    _Generic((x) * (y),                                                                            \
        long double: nextafterl(x, y),                                                             \
        double: nextafter(x, y),                                                                   \
        float: nextafterf(x, y),                                                                   \
        default: nextafter((double)x, (double)y))

#define nexttoward(x, y)                                                                           \
    _Generic((x),                                                                                  \
        long double: nexttowardl(x, y),                                                            \
        double: nexttoward(x, y),                                                                  \
        float: nexttowardf(x, y),                                                                  \
        default: nexttoward((double)x, y))

#define pow(x, y)                                                                                  \
    _Generic((x) * (y),                                                                            \
        long double complex: cpowl(x, y),                                                          \
        double complex: cpow(x, y),                                                                \
        float complex: cpowf(x, y),                                                                \
        long double: powl(x, y),                                                                   \
        double: pow(x, y),                                                                         \
        float: powf(x, y),                                                                         \
        default: pow((double)x, (double)y))

#define remainder(x, y)                                                                            \
    _Generic((x) * (y),                                                                            \
        long double: remainderl(x, y),                                                             \
        double: remainder(x, y),                                                                   \
        float: remainderf(x, y),                                                                   \
        default: remainder((double)x, (double)y))

#define remquo(x, y, z)                                                                            \
    _Generic((x) * (y),                                                                            \
        long double: remquol(x, y, z),                                                             \
        double: remquo(x, y, z),                                                                   \
        float: remquof(x, y, z),                                                                   \
        default: remquo((double)x, (double)y, z))

#define rint(x)                                                                                    \
    _Generic((x), long double: rintl(x), double: rint(x), float: rintf(x), default: rint((double)x))

#define round(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double: roundl(x),                                                                    \
        double: round(x),                                                                          \
        float: roundf(x),                                                                          \
        default: round((double)x))

#define scalbln(x, y)                                                                              \
    _Generic((x),                                                                                  \
        long double: scalblnl(x, y),                                                               \
        double: scalbln(x, y),                                                                     \
        float: scalblnf(x, y),                                                                     \
        default: scalbln((double)x, y))

#define scalbn(x, y)                                                                               \
    _Generic((x),                                                                                  \
        long double: scalbnl(x, y),                                                                \
        double: scalbn(x, y),                                                                      \
        float: scalbnf(x, y),                                                                      \
        default: scalbn((double)x, y))

#define sin(x)                                                                                     \
    _Generic((x),                                                                                  \
        long double complex: csinl(x),                                                             \
        double complex: csin(x),                                                                   \
        float complex: csinf(x),                                                                   \
        long double: sinl(x),                                                                      \
        double: sin(x),                                                                            \
        float: sinf(x),                                                                            \
        default: sin((double)x))

#define sinh(x)                                                                                    \
    _Generic((x),                                                                                  \
        long double complex: csinhl(x),                                                            \
        double complex: csinh(x),                                                                  \
        float complex: csinhf(x),                                                                  \
        long double: sinhl(x),                                                                     \
        double: sinh(x),                                                                           \
        float: sinhf(x),                                                                           \
        default: sinh((double)x))

#define sqrt(x)                                                                                    \
    _Generic((x),                                                                                  \
        long double complex: csqrtl(x),                                                            \
        double complex: csqrt(x),                                                                  \
        float complex: csqrtf(x),                                                                  \
        long double: sqrtl(x),                                                                     \
        double: sqrt(x),                                                                           \
        float: sqrtf(x),                                                                           \
        default: sqrt((double)x))

#define tan(x)                                                                                     \
    _Generic((x),                                                                                  \
        long double complex: ctanl(x),                                                             \
        double complex: ctan(x),                                                                   \
        float complex: ctanf(x),                                                                   \
        long double: tanl(x),                                                                      \
        double: tan(x),                                                                            \
        float: tanf(x),                                                                            \
        default: tan((double)x))

#define tanh(x)                                                                                    \
    _Generic((x),                                                                                  \
        long double complex: ctanhl(x),                                                            \
        double complex: ctanh(x),                                                                  \
        float complex: ctanhf(x),                                                                  \
        long double: tanhl(x),                                                                     \
        double: tanh(x),                                                                           \
        float: tanhf(x),                                                                           \
        default: tanh((double)x))

#define tgamma(x)                                                                                  \
    _Generic((x),                                                                                  \
        long double: tgammal(x),                                                                   \
        double: tgamma(x),                                                                         \
        float: tgammaf(x),                                                                         \
        default: tgamma((double)x))

#define trunc(x)                                                                                   \
    _Generic((x),                                                                                  \
        long double: truncl(x),                                                                    \
        double: trunc(x),                                                                          \
        float: truncf(x),                                                                          \
        default: trunc((double)x))

#endif /* !_TGMATH_H */
