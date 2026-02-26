#ifndef GLOB_H
#define GLOB_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 2)

#include <stddef.h>

typedef struct {
    size_t gl_pathc;
    char **gl_pathv;
    size_t gl_offs;
} glob_t;

#define GLOB_APPEND   0x0001
#define GLOB_DOOFFS   0x0002
#define GLOB_ERR      0x0004
#define GLOB_MARK     0x0008
#define GLOB_NOCHECK  0x0010
#define GLOB_NOESCAPE 0x0020
#define GLOB_NOSORT   0x0040

#define GLOB_ABORTED  1
#define GLOB_NOMATCH  2
#define GLOB_NOSPACE  3

int glob(const char *pattern, int flags, int (*errfunc)(const char *epath, int eerrno), glob_t *pglob);
void globfree(glob_t *pglob);

#endif /* !_ANSI && (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 2) */

#endif /* !GLOB_H */
