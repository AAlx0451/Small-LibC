#ifndef MEMORY_H
#define MEMORY_H

#include <features.h>

#if !defined(_ANSI) && (defined(_GNU_SOURCE) || defined(_XOPEN_SOURCE))
#include <stddef.h>

#warning <memory.h> is deprecated, use <string.h>

void *memccpy(void *restrict dest, const void *restrict src, int c, size_t n);
void *memchr(const void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);

#endif /* !_ANSI && (_GNU_SOURCE || _XOPEN_SOURCE) */

#endif /* !MEMORY_H */
