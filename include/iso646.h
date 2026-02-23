#ifndef ISO646_H
#define ISO646_H

#include <features.h>

# if !defined(_ANSI) && ((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L))

#  ifndef __cplusplus /* to suppress errrors */
#   define and &&
#   define and_eq &=
#   define bitand &
#   define bitor |
#   define compl ~
#   define not !
#   define not_eq !=
#   define or ||
#   define or_eq |=
#   define xor ^
#   define xor_eq ^=
#  endif /* !__cplusplus */

# endif /* !_ANSI && ((_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || _XOPEN_SOURCE && _XOPEN_SOURCE >= 600) || (__STDC_VERSION__ && __STDC_VERSION__ >= 199901L)) */
#endif /* !ISO646_H */
