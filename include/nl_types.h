#ifndef _NL_TYPES_H
#define _NL_TYPES_H

#include <features.h>

#if !defined(_ANSI) &&                                                                             \
    ((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || defined(_XOPEN_SOURCE))

# define NL_SETD       1
# define NL_CAT_LOCALE 1

typedef int nl_item;
typedef void *nl_catd;

/* TODO: very later
nl_catd catopen(const char *, int);
char *catgets(nl_catd, int, int, const char *);
int catclose(nl_catd);
*/

#endif /* !_ANSI && ((_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || _XOPEN_SOURCE) */

#endif /* !_NL_TYPES_H */
