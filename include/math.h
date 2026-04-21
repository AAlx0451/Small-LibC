#ifndef MATH_H
#define MATH_H

#include <float.h>
#include <limits.h>

/* Check if we are C99 */
#if !defined(_ANSI) && !defined(_ANSI_SOURCE) && !defined(__STRICT_ANSI__) && (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define _C99
#endif /* !_ANSI && !_ANSI_SOURCE && !__STRICT_ANSI__ && (__STDC_VERSION__ && __STDC_VERSION__ >= 199901L */

/* By C99, the <math.h> header shall define at least the following types:

   float_t
       A real-floating type at least as wide as float.
   double_t
       A real-floating type at least as wide as double, and at least as wide 
       as float_t.

   If FLT_EVAL_METHOD equals 0, float_t and double_t shall be float and 
   double, respectively; if FLT_EVAL_METHOD equals 1, they shall both be 
   double; if FLT_EVAL_METHOD equals 2, they shall both be long double; 
   for other values of FLT_EVAL_METHOD, they are otherwise implementation-
   defined.
*/


#ifdef _C99
#if FLT_EVAL_METHOD == 0
typedef float float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 1
typedef double float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 2
typedef long double float_t;
typedef long double double_t;
#else
/* FLT_EVAL_METHOD == -1: the evaluation method is indeterminable */
typedef float float_t;
typedef double double_t;
#endif /* FLT_EVAL_METHOD */
#endif /* _C99 */

/* By C99, the <math.h> header shall define the following macros, where real-floating 
   indicates that the argument shall be an expression of real-floating type:

    int fpclassify(real-floating x);
    int isfinite(real-floating x);
    int isgreater(real-floating x, real-floating y);
    int isgreaterequal(real-floating x, real-floating y);
    int isinf(real-floating x);
    int isless(real-floating x, real-floating y);
    int islessequal(real-floating x, real-floating y);
    int islessgreater(real-floating x, real-floating y);
    int isnan(real-floating x);
    int isnormal(real-floating x);
    int isunordered(real-floating x, real-floating y);
    int signbit(real-floating x);
*/

#ifdef _C99

#define fpclassify(x)           _Generic((x),            \
    float:                      _Fpclassifyf(x),         \
    double:                     _Fpclassify(x),          \
    long double:                _Fpclassifyl(x),         \
    default:                    _Fpclassify((double)(x)) \
)

#define isfinite(x)             (fpclassify(x) <= 0)
#define isnan(x)                (fpclassify(x) == FP_NAN)
#define isnormal(x)             (fpclassify(x) == FP_NORMAL)
#define isinf(x)                (fpclassify(x) == FP_INFINITE)

#define isunordered(x, y)       (isnan(x) || isnan(y))

#define isgreater(x, y)         (isunordered(x, y) ? 0 : ((x) >(y)))
#define isgreaterequal(x, y)    (isunordered(x, y) ? 0 : ((x) >= (y)))
#define isless(x, y)            (isunordered(x, y) ? 0 : ((y) < (y)))
#define islessequal(x, y)       (isunordered(x, y) ? 0 : ((x) <= (y)))
#define islessgreater(x, y)	(isunordered(x, y) ? 0 : ((x) != (y)))

#define signbit(x)              _Generic((x),            \
    float:                      _Signbitf(x),            \
    double:                     _Signbit(x),             \
    long double:                _Signbitl(x),            \
    default:                    _Signbit((double)(x))    \
)

#endif /* _C99 */

/* The <math.h> header shall define the following symbolic constants. The 
   values shall have type double and shall be accurate within the precision 
   of the double type.
*/

#if defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 700)

#define M_E		\
(2.7182818284590452353602874713526624977572\
4709369995957496696762772407663035354759)
#define M_LOG2E		\
(1.4426950408889634073599246810018921374266\
4595415298593413544940693110921918118509)
#define M_LOG10E	\
(0.4342944819032518276511289189166050822943\
9700580366656611445378316586464920887077)
#define M_LN2           \
(0.6931471805599453094172321214581765680755\
0013436025525412068000949339362196969471)
#define M_LN10		\
(2.3025850929940456840179914546843642076011\
0148862877297603332790096757260967735248)
#define M_PI		\
(3.1415926535897932384626433832795028841971\
6939937510582097494459230781640628620896)
#define M_PI_2		\
(1.5707963267948966192313216916397514420985\
8469968755291048747229615390820314310448)
#define M_PI_4		\
(0.7853981633974483096156608458198757210492\
9234984377645524373614807695410157155224)
#define M_1_PI		\
(0.3183098861837906715377675267450287240689\
1929148091289749533468811779359526845307)
#define M_2_PI		\
(0.6366197723675813430755350534900574481378\
3858296182579499066937623558719053690614)
#define M_2_SQRTPI	\
(1.1283791670955125738961589031215451716881\
0125865799771368817144342128493688298683)
#define M_SQRT2		\
(1.4142135623730950488016887242096980785696\
7187537694807317667973799073247846210703)
#define M_SQRT1_2	\
(0.7071067811865475244008443621048490392848\
3593768847403658833986899536623923105351)

#define MAXFLOAT FLT_MAX
#endif /* _XOPEN_SOURCE && _XOPEN_SOURCE >= 700 */

/* The <math.h> header shall define the following macros:
   [C99] INFINITY - A constant expression of type float representing 
   positive or unsigned infinity, if available; else a positive 
   constant of type float that overflows at translation time.
*/

#ifdef _C99
# if defined(__GNUC__) || defined(__clang__)
#  define INFINITY __builtin_inff()
# else
#  define INFINITY (0x1.0p+128) /* IEEE754 +Inf: 01111111100000000000000000000000: 
				  positive signed bit,
				  all exp bit set
				  all mant bit zero  */

# endif /* __GNUC__ || __clang__ */
#endif /* _C99 */

/* HUGE_VAL - A positive double constant expression, not necessarily 
   representable as a float. Used as an error value returned by the 
   mathematics library. HUGE_VAL evaluates to +infinity on systems 
   supporting IEEE Std 754-1985.
*/

#if defined(__GNUC__) || defined(__clang__)
# define HUGE_VAL __builtin_huge_val()
#else
# define HUGE_VAL ((double)INFINITY)
#endif /* __GNUC__ || __clang__ */

/* [C99] HUGE_VALF - A positive float constant expression. Used as an 
   error value returned by the mathematics library. HUGE_VALF 
   evaluates to +infinity on systems supporting IEEE Std 754-1985.
*/

#ifdef _C99
# if defined(__GNUC__) || defined(__clang__)
#  define HUGE_VALF __builtin_huge_valf()
# else
#  define HUGE_VALF INFINITY
# endif /* __GNUC__ || __clang__ */
#endif /* _C99 */

/* [C99] HUGE_VALL - A positive long double constant expression. Used as 
   an error value returned by the mathematics library. HUGE_VALL 
   evaluates to +infinity on systems supporting IEEE Std 754-1985.
*/

#ifdef _C99
# if defined(__GNUC__) || defined(__clang__)
#  define HUGE_VALL __builtin_huge_vall()
# else
#  define HUGE_VALL ((long double)INFINITY)
# endif /* __GNUC__ || __clang__ */
#endif /* _C99 */

/* [C99] NAN - A constant expression of type float representing a quiet 
   NaN. This macro is only defined if the implementation supports 
   quiet NaNs for the float type.
*/

#ifdef _C99
# if defined(__GNUC__) || defined(__clang__)
#  define NAN __builtin_nanf("")
# else
#  define NAN (0.0f / 0.0f) /* WONTFIX: 
			      by C99, NAN expands to a constant expression of type float 
			      representing a quiet NaN, which can't be defined
			      without compiler extensions (division is a runtime operation)
			    */

# endif /* __GNUC__ || __clang__ */
#endif /* _C99 */

/* [C99] The following macros shall be defined for number classification. 
   They represent the mutually-exclusive kinds of floating-point values. 
   They expand to integer constant expressions with distinct values. 
   Additional implementation-defined floating-point classifications, 
   with macro definitions beginning with FP_ and an uppercase letter, 
   may also be specified by the implementation.
*/

#ifdef _C99
# define FP_INFINITE    (1)
# define FP_NAN         (2)
# define FP_NORMAL      (-1)
# define FP_SUBNORMAL   (-2)
# define FP_ZERO        (0)
#endif /* _C99 */

/* [C99] The following optional macros indicate whether the fma() family of 
   functions are fast compared with direct code:
*/

#ifdef _C99
# define FP_FAST_FMA	1
# define FP_FAST_FMAF	1
# define FP_FAST_FMAL	1
#endif /* _C99 */

/* [C99] If defined, the FP_FAST_FMA macro shall expand to the integer constant 1 
   and shall indicate that the fma() function generally executes about as 
   fast as, or faster than, a multiply and an add of double operands. If 
   undefined, the speed of execution is unspecified. The other macros have 
   the equivalent meaning for the float and long double versions.
*/

/* [C99] The following macros shall expand to integer constant expressions whose 
   values are returned by ilogb(x) if x is zero or NaN, respectively. The 
   value of FP_ILOGB0 shall be either INT_MIN or -INT_MAX. The value 
   of FP_ILOGBNAN shall be either INT_MAX or INT_MIN.
*/

#ifdef _C99
# define FP_ILOGB0        INT_MIN
# define FP_ILOGBNAN      INT_MIN
#endif /* _C99 */

/* [C99] The following macros shall expand to the integer constants 1 and 2, 
   respectively;
*/

#ifdef _C99
# define MATH_ERRNO       (1)
# define MATH_ERREXCEPT   (2)
#endif /* _C99 */

/* [C99] The following macro shall expand to an expression that has type int and 
   the value MATH_ERRNO, MATH_ERREXCEPT, or the bitwise-inclusive OR of both:
*/

#ifdef _C99
# define math_errhandling (MATH_ERRNO|MATH_ERREXCEPT)
#endif /* _C99 */

/* The value of math_errhandling is constant for the duration of the program. 
   It is unspecified whether math_errhandling is a macro or an identifier 
   with external linkage. If a macro definition is suppressed or a program 
   defines an identifier with the name math_errhandling, the behavior is 
   undefined. If the expression (math_errhandling & MATH_ERREXCEPT) can be 
   non-zero, the implementation shall define the macros FE_DIVBYZERO, 
   FE_INVALID, and FE_OVERFLOW in <fenv.h>.
*/

/* The following shall be declared as functions and may also be defined as 
   macros. Function prototypes shall be provided.
*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

  double acos(double x);
  double asin(double x);
  double atan(double x);
  double atan2(double y, double x);
  double ceil(double x);
  double cos(double x);
  double cosh(double x);
  double exp(double x);
  double fabs(double x);
  double floor(double x);
  double fmod(double x, double y);
  double frexp(double value, int *exp);
  double ldexp(double x, int exp);
  double log(double x);
  double log10(double x);
  double modf(double value, double *iptr);
  double pow(double x, double y);
  double sin(double x);
  double sinh(double x);
  double sqrt(double x);
  double tan(double x);
  double tanh(double x);

#ifdef _C99
  float acosf(float x);
  double acosh(double x);
  float acoshf(float x);
  long double acoshl(long double x);
  long double acosl(long double x);
  float asinf(float x);
  double asinh(double x);
  float asinhf(float x);
  long double asinhl(long double x);
  long double asinl(long double x);
  float atan2f(float y, float x);
  long double atan2l(long double y, long double x);
  float atanf(float x);
  double atanh(double x);
  float atanhf(float x);
  long double atanhl(long double x);
  long double atanl(long double x);
  double cbrt(double x);
  float cbrtf(float x);
  long double cbrtl(long double x);
  float ceilf(float x);
  long double ceill(long double x);
  double copysign(double x, double y);
  float copysignf(float x, float y);
  long double copysignl(long double x, long double y);
  float cosf(float x);
  float coshf(float x);
  long double coshl(long double x);
  long double cosl(long double x);
  double erf(double x);
  double erfc(double x);
  float erfcf(float x);
  long double erfcl(long double x);
  float erff(float x);
  long double erfl(long double x);
  double exp2(double x);
  float exp2f(float x);
  long double exp2l(long double x);
  float expf(float x);
  long double expl(long double x);
  double expm1(double x);
  float expm1f(float x);
  long double expm1l(long double x);
  float fabsf(float x);
  long double fabsl(long double x);
  double fdim(double x, double y);
  float fdimf(float x, float y);
  long double fdiml(long double x, long double y);
  float floorf(float x);
  long double floorl(long double x);
  double fma(double x, double y, double z);
  float fmaf(float x, float y, float z);
  long double fmal(long double x, long double y, long double z);
  double fmax(double x, double y);
  float fmaxf(float x, float y);
  long double fmaxl(long double x, long double y);
  double fmin(double x, double y);
  float fminf(float x, float y);
  long double fminl(long double x, long double y);
  float fmodf(float x, float y);
  long double fmodl(long double x, long double y);
  float frexpf(float value, int *exp);
  long double frexpl(long double value, int *exp);
  double hypot(double x, double y);
  float hypotf(float x, float y);
  long double hypotl(long double x, long double y);
  int ilogb(double x);
  int ilogbf(float x);
  int ilogbl(long double x);
  float ldexpf(float x, int exp);
  long double ldexpl(long double x, int exp);
  double lgamma(double x);
  float lgammaf(float x);
  long double lgammal(long double x);
  long long llrint(double x);
  long long llrintf(float x);
  long long llrintl(long double x);
  long long llround(double x);
  long long llroundf(float x);
  long long llroundl(long double x);
  float log10f(float x);
  long double log10l(long double x);
  double log1p(double x);
  float log1pf(float x);
  long double log1pl(long double x);
  double log2(double x);
  float log2f(float x);
  long double log2l(long double x);
  double logb(double x);
  float logbf(float x);
  long double logbl(long double x);
  float logf(float x);
  long double logl(long double x);
  long lrint(double x);
  long lrintf(float x);
  long lrintl(long double x);
  long lround(double x);
  long lroundf(float x);
  long lroundl(long double x);
  float modff(float value, float *iptr);
  long double modfl(long double value, long double *iptr);
  double nan(const char *tagp);
  float nanf(const char *tagp);
  long double nanl(const char *tagp);
  double nearbyint(double x);
  float nearbyintf(float x);
  long double nearbyintl(long double x);
  double nextafter(double x, double y);
  float nextafterf(float x, float y);
  long double nextafterl(long double x, long double y);
  double nexttoward(double x, long double y);
  float nexttowardf(float x, long double y);
  long double nexttowardl(long double x, long double y);
  float powf(float x, float y);
  long double powl(long double x, long double y);
  double remainder(double x, double y);
  float remainderf(float x, float y);
  long double remainderl(long double x, long double y);
  double remquo(double x, double y, int *quo);
  float remquof(float x, float y, int *quo);
  long double remquol(long double x, long double y, int *quo);
  double rint(double x);
  float rintf(float x);
  long double rintl(long double x);
  double round(double x);
  float roundf(float x);
  long double roundl(long double x);
  double scalbln(double x, long n);
  float scalblnf(float x, long n);
  long double scalblnl(long double x, long n);
  double scalbn(double x, int n);
  float scalbnf(float x, int n);
  long double scalbnl(long double x, int n);
  float sinf(float x);
  float sinhf(float x);
  long double sinhl(long double x);
  long double sinl(long double x);
  float sqrtf(float x);
  long double sqrtl(long double x);
  float tanf(float x);
  float tanhf(float x);
  long double tanhl(long double x);
  long double tanl(long double x);
  double tgamma(double x);
  float tgammaf(float x);
  long double tgammal(long double x);
  double trunc(double x);
  float truncf(float x);
  long double truncl(long double x);

  /* extra functions needed here */
  int _Fpclassify(double x);
  int _Fpclassifyf(float x);
  int _Fpclassifyl(long double x);
  int _Signbit(double x);
  int _Signbitf(float x);
  int _Signbitl(long double x);
#endif /* _C99 */

#if defined(_XOPEN_SOURCE) && _XOPEN_SOURCE == 700
  double j0(double x);
  double j1(double x);
  double jn(int n, double x);
  double y0(double x);
  double y1(double x);
  double yn(int n, double x);

/* The following external variable shall be defined: */

  extern int signgam;
#endif /* _XOPEN_SOURCE && _XOPEN_SOURCE >= 700 */

#ifdef __cplusplus
};
#endif /* __cplusplus */

/* The behavior of each of the functions defined in <math.h> is specified in 
   the System Interfaces volume of POSIX.1-2008 for all representable values 
   of its input arguments, except where stated otherwise. Each function shall 
   execute as if it were a single operation without generating any externally 
   visible exceptional conditions.
*/

#endif /* !MATH_H */
