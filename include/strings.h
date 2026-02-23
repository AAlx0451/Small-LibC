#ifndef STRINGS_H
#define STRINGS_H

#include <features.h>

#if !defined(_ANSI) && (defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE) || defined(_XOPEN_SOURCE))

#include <stddef.h>
#include <string.h>

int bcmp(const void *s1, const void *s2, size_t n);
void bcopy(const void *src, void *dest, size_t n);
void bzero(void *s, size_t n);
int ffs(int i);
char *index(const char *s, int c);
char *rindex(const char *s, int c);
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);

#endif /* !_ANSI && (_GNU_SOURCE || _DARWIN_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !STRINGS_H */
