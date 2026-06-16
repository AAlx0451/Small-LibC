#ifndef _STDARG_H
#define _STDARG_H

#include <features.h>

#if defined(__GNUC__) || defined(__clang__)

typedef __builtin_va_list va_list;

# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define va_start(...) __builtin_va_start(__VA_ARGS__)
# else
#  define va_start(ap, parmN) __builtin_va_start(ap, parmN)
# endif /* __STDC_VERSION__ >= 202311L */

# define va_end(ap) __builtin_va_end(ap)
# define va_arg(ap, type) __builtin_va_arg(ap, type)

# if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) ||                                 \
    (defined(__cplusplus) && __cplusplus >= 201103L)
#  define va_copy(dest, src) __builtin_va_copy(dest, src)
# endif /* (__STDC_VERSION__ && __STDC_VERSION__ >= 199901L) || (__cplusplus && __cplusplus >= 201103L)  */

#else

/* not supported! */
# warning "<stddef.h> not supported for this compiler"

#endif /* __GNUC__ || __clang__ */

#endif /* _STDARG_H */
