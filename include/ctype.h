#ifndef CTYPE_H
#define CTYPE_H

#include <features.h>

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c); 

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
     _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
int isblank(int c);
#endif /* (__STDC_VERSION__ && __STDC_VERSION__ >= 199001L) || _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600 */

#if !defined(_ANSI) && (defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE))

# define _tolower(c) ((c) | 0x20)
# define _toupper(c) ((c) & ~0x20)

#endif /* !_ANSI && (_XOPEN_SOURCE || _GNU_SOURCE || _DARWIN_C_SOURCE) */

#endif /* !CTYPE_H */
