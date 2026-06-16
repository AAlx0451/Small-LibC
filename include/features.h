#ifndef _FEATURES_H
#define _FEATURES_H

#if defined(__GNUC__) || defined(__clang__)
# define __always_inline          __attribute__((always_inline))
# define __cold                   __attribute__((cold))
# define __deprecated             __attribute__((deprecated))
# define __slibc_has_attribute(M) __has_attribute(M)
# define __inline                 __inline__
# define __naked                  __attribute__((naked))
# define __noreturn               __attribute__((noreturn))
# define __noinline               __attribute__((noinline))
# define __restrict               __restrict__
# define __unreachable()          __builtin_unreachable()
# define __unused                 __attribute__((unused))
# define __used                   __attribute__((used))
#else
# define __always_inline
# define __cold
# define __deprecated
# define __inline
# define __slibc_has_attribute(M) 0
# define __naked
# define __noreturn
# define __noinline
# define ___restrict
# define __unreachable() ((void)0)
# define __unused
# define __used
#endif /* __GNUC__ || __clang__ */
#if __slibc_has_attribute(fallthrough)
# define __fallthrough __attribute__((fallthrough))
#else
# define __fallthrough ((void)0)
#endif /* __slibc_has_attribute(fallthrough) */
#if defined(__ASSEMBLER__) || defined(ASSEMBLER) || defined(_ASM_SOURCE)
# ifndef _ASM_SOURCE
#  define _ASM_SOURCE
# endif /* !_ASM_SOURCE */
#else
# define _C_SOURCE /* used at mach only */
#endif             /* __ASSEMBLER__ || ASSEMBLER || _ASM_SOURCE */
#if defined(_ANSI_SOURCE) || defined(__STRICT_ANSI__)
# undef _POSIX_C_SOURCE
# undef _POSIX_SOURCE
# undef _GNU_SOURCE
# undef _XOPEN_SOURCE
# undef _DARWIN_C_SOURCE
# define _ANSI
# if defined(__GNUC__) || defined(__clang__)
#  define inline   __inline
#  define restrict __restrict
# else
#  define inline
#  define restrict
# endif /* __GNUC__ || __clang__ */
#else
# if !defined(_XOPEN_SOURCE) && !defined(_POSIX_C_SOURCE) && !defined(_POSIX_SOURCE) &&            \
     !defined(_GNU_SOURCE) && !defined(_DARWIN_C_SOURCE)
#  define _GNU_SOURCE
#  define _DARWIN_C_SOURCE
# endif /* !_XOPEN_SOURCE && !_POSIX_C_SOURCE && !_POSIX_SOURCE && !_GNU_SOURCE &&                 \
           !_DARWIN_C_SOURCE */
# if defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE)
#  define _POSIX_C_SOURCE 200809L
#  define _XOPEN_SOURCE   700
#  define SMALL_LIBC
#  define _MACH_SOURCE
#  define __deconst(type, val) ((type)(uintptr_t)(const void *)(val))
# elif defined(_XOPEN_SOURCE)
#  if _XOPEN_SOURCE < 500 && (_POSIX_C_SOURCE < 2L)
#   ifndef _POSIX_SOURCE
#    define _POSIX_SOURCE
#   endif /* !_POSIX_SOURCE */
#   define _POSIX_C_SOURCE 2L
#  elif _XOPEN_SOURCE == 500 && (_POSIX_C_SOURCE <= 199506L || !defined(_POSIX_C_SOURCE))
#   define _POSIX_C_SOURCE 199506L
#  elif _XOPEN_SOURCE == 600 && (_POSIX_C_SOURCE <= 200112L || !defined(_POSIX_C_SOURCE))
#   define _POSIX_C_SOURCE 200112L
#  elif _XOPEN_SOURCE == 700 && (_POSIX_C_SOURCE <= 200809L || !defined(_POSIX_C_SOURCE))
#   define _POSIX_C_SOURCE 200809L
#  endif /* _XOPEN_SOURCE < 500 && (_POSIX_C_SOURCE < 2L)  */
# endif  /* _DARWIN_C_SOURCE || _GNU_SOURCE */
# if defined(_POSIX_SOURCE) && !defined(_POSIX_C_SOURCE)
#  define _POSIX_C_SOURCE 1L
# endif /* _POSIX_SOURCE && !_POSIX_C_SOURCE */
#endif  /* _ANSI_SOURCE || __STRICT_ANSI__ */

#endif /* !_FEATURES_H */
