#include "malloc.h"
/* #include <string.h> */ // TODO, now at malloc.h
#include <stdint.h>

void *calloc(size_t number, size_t size)
{
     if (number == 0 || size == 0) {
        return malloc(0);
    }
    if (size > SIZE_MAX / number) {
        return NULL;
    }
    size_t total_size = number * size;
    void *ptr = malloc(total_size);
    if (ptr != NULL)
    {
        memset(ptr, 0, total_size);
    }
    return ptr;
}
