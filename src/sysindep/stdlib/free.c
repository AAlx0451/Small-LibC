#include "malloc.h"

void free(void *ptr)
{
    meta_ptr block;

    if (!ptr) {
        return;
    }

    while (__sync_lock_test_and_set(&malloc_lock, 1)) {
        while (malloc_lock);
    }

    block = get_block_ptr(ptr);

    if (!is_valid_block(block) || block->magic == MAGIC_FREE) {
        __sync_lock_release(&malloc_lock);
        return;
    }

    block->magic = MAGIC_FREE;
    coalesce(block);
    
    __sync_lock_release(&malloc_lock);
}
