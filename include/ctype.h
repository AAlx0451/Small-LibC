#ifndef _CTYPE_H
#define _CTYPE_H

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
    defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE)
int isblank(int c);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)

#define _tolower(c) ((c) | 0x20)
#define _toupper(c) ((c) & ~0x20)

#endif /* _XOPEN_SOURCE || _GNU_SOURCE */

#endif
