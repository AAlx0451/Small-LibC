#include "malloc.h"

void *calloc(size_t number, size_t size) {
    size_t total_size;
    void *ptr;

    if(number == 0 || size == 0) {
        return NULL;
    }

    if(__builtin_mul_overflow(number, size, &total_size)) {
        return NULL;
    }

    ptr = malloc(total_size);
    if(ptr) {
        memset(ptr, 0, total_size);
    }

    return ptr;
}
