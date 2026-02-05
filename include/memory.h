#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#warning <memory.h> is deprecated, use <string.h>

void *memccpy(void *restrict dest, const void *restrict src, int c, size_t n);
void *memchr(const void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);

#endif // MEMORY_H
