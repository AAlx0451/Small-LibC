#ifndef FNMATCH_H
#define FNMATCH_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 2)

#define FNM_NOMATCH 1

#define FNM_NOESCAPE 0x01
#define FNM_PATHNAME 0x02
#define FNM_PERIOD 0x04

int fnmatch(const char *pattern, const char *string, int flags);

#endif /* !_ANSI && (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 2) */

#endif /* !FNMATCH_H */
