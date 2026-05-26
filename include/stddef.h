#ifndef STDDEF_H
#define STDDEF_H

#include <features.h>

#undef NULL
#if defined(__cplusplus)
# if __cplusplus >= 201103L
#  define NULL nullptr
# else
#  define NULL 0
# endif /* __cplusplus >= 201103L */
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
# define NULL nullptr
#else
# define NULL ((void *)0)
#endif /* __cplusplus */

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;

#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif /* !__cplusplus */

#undef offsetof
#if defined(__GNUC__) || defined(__clang__)
# define offsetof(type, member) __builtin_offsetof(type, member)
#else
# define offsetof(type, member) ((size_t)&(((type *)0)->member))
#endif /* __GNUC__ || __clang__ */

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) ||                                  \
    (defined(__cplusplus) && __cplusplus >= 201103L)
typedef double max_align_t;
#endif /* (__STDC_VERSION__ && __STDC_VERSION__ >= 201112L) || (__cplusplus && __cplusplus >=      \
          201103L) */

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) ||                                  \
    (defined(__cplusplus) && __cplusplus >= 201103L)
typedef typeof(nullptr) nullptr_t;
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
# ifndef unreachable
#  if defined(__GNUC__) || defined(__clang__)
#   define unreachable() __builtin_unreachable()
#  else
#   define unreachable() ((void)0)
#  endif /* __GNUC__ || __clang__ */
# endif  /* !unreachable */
#endif   /* __STDC_VERSION__ && __STDC_VERSION__ >= 202311L */

#endif /* _STDDEF_H */
