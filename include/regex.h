#ifndef REGEX_H
#define REGEX_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 2)

# include <sys/types.h>

# define REG_EXTENDED 1
# define REG_ICASE    2
# define REG_NOSUB    4
# define REG_NEWLINE  8

# define REG_NOTBOL 1
# define REG_NOTEOL 2

# define REG_NOMATCH  1
# define REG_BADPAT   2
# define REG_ECOLLATE 3
# define REG_ECTYPE   4
# define REG_EESCAPE  5
# define REG_ESUBREG  6
# define REG_EBRACK   7
# define REG_EPAREN   8
# define REG_EBRACE   9
# define REG_BADBR    10
# define REG_ERANGE   11
# define REG_ESPACE   12
# define REG_BADRPT   13

typedef ssize_t regoff_t;

typedef struct {
    size_t re_nsub;
    void *re_comp;
    int re_cflags;
} regex_t;

typedef struct {
    regoff_t rm_so;
    regoff_t rm_eo;
} regmatch_t;

int regcomp(regex_t *, const char *, int);
int regexec(const regex_t *, const char *, size_t, regmatch_t[], int);
size_t regerror(int, const regex_t *, char *, size_t);
void regfree(regex_t *);

#endif /* !_ANSI && (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 2) */

#endif /* !REGEX_H */
