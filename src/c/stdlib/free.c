#include "malloc.h"

void free(void *ptr)
{
    meta_ptr block;

    if (!ptr) {
        return;
    }

    if (((uintptr_t)ptr & (ALIGNMENT - 1)) != 0) {
        return;
    }

    __malloc_spin_lock(&malloc_lock);

    block = get_block_ptr(ptr);

    if (!is_valid_block(block) || block->magic != MAGIC_USED) {
        __malloc_spin_unlock(&malloc_lock);
        return;
    }

    block->magic = MAGIC_FREE;
    coalesce(block);

    __malloc_spin_unlock(&malloc_lock);
}
