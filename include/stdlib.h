#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

void free(void *ptr);
void *malloc(size_t size);
void *calloc(size_t number, size_t size);
void *realloc(void *p, size_t size);

#endif
