#ifndef  _TYPES_H
#define _TYPES_H

# if !defined(_ANSI) && ((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409LL))

typedef struct
{
    int __count;
    char __bytes[4];
} mbstate_t;

# endif /* !_ANSI && ((_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || _XOPEN_SOURCE && _XOPEN_SOURCE >= 600) || (__STDC_VERSION__ && __STDC_VERSION__ >= 199409L)) */

#endif /* !_TYPES_H */
