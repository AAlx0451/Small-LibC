#ifndef FEATURES_H
# define FEATURES_H

#if defined(_ANSI_SOURCE) || defined(__STRICT_ANSI__)
# undef _POSIX_C_SOURCE
# undef _POSIX_SOURCE
# undef _GNU_SOURCE
# undef _XOPEN_SOURCE
# undef _DARWIN_C_SOURCE
# define _ANSI
# ifndef inline
#  define inline
# endif
# ifndef restrict
#  define restrict
# endif

#else 

# if !defined(_XOPEN_SOURCE) && !defined(_POSIX_C_SOURCE) && !defined(_POSIX_SOURCE) \
    && !defined(_GNU_SOURCE) && !defined(_DARWIN_C_SOURCE)
#  define _GNU_SOURCE
#  define _DARWIN_C_SOURCE
# endif

# if defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE)
#  define _POSIX_C_SOURCE 200809L
#  define _XOPEN_SOURCE 700
# elif defined(_XOPEN_SOURCE) 
#  if _XOPEN_SOURCE < 500 && (_POSIX_C_SOURCE <= 2 || !defined(_POSIX_C_SOURCE))
#   define _POSIX_SOURCE
#  elif _XOPEN_SOURCE == 500 && (_POSIX_C_SOURCE <= 199506L || !defined(_POSIX_C_SOURCE))
#   define _POSIX_C_SOURCE 199506L
#  elif _XOPEN_SOURCE == 600 && (_POSIX_C_SOURCE <= 200112L || !defined(_POSIX_C_SOURCE))
#   define _POSIX_C_SOURCE 200112L
#  elif _XOPEN_SOURCE == 700 && (_POSIX_C_SOURCE <= 200809L || !defined(_POSIX_C_SOURCE))
#   define _POSIX_C_SOURCE 200809L
#  endif 
# endif

# if defined(_POSIX_SOURCE) && !defined(_POSIX_C_SOURCE)
#  define _POSIX_C_SOURCE 1L
# endif

#endif

#endif
