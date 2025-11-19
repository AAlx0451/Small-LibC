#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char *str);
void *memset(void *ptr, int x, size_t n);
void *memmove(void *dest, const void *src, size_t count);
void *memcpy(void *dest, const void *src, size_t n);

#define NULL ((void*)0)

#endif
