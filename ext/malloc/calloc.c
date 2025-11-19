#include "malloc.h"

void *calloc(size_t number, size_t size)
{
    size_t total_size;
    void *ptr;

    if (number == 0 || size == 0) {
        return NULL;
    }

    if (size > SIZE_MAX / number) {
        return NULL;
    }

    total_size = number * size;
    ptr = malloc(total_size);

    if (ptr) {
        memset(ptr, 0, total_size);
    }

    return ptr;
}
