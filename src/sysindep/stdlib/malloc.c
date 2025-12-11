#include "malloc.h"

meta_ptr heap_base = NULL;
volatile int malloc_lock = 0;

void *malloc(size_t size)
{
    meta_ptr block, last;
    size_t aligned_size;

    if (size == 0) {
        return NULL;
    }

    aligned_size = ALIGN(size);

    while (__sync_lock_test_and_set(&malloc_lock, 1)) {
        while (malloc_lock);
    }

    block = heap_base;
    last = NULL;

    while (block) {
        if (block->magic == MAGIC_FREE && block->size >= aligned_size) {
            block->magic = MAGIC_USED;
            split_block(block, aligned_size);
            __sync_lock_release(&malloc_lock);
            return (void *)((char *)block + BLOCK_META_SIZE);
        }
        last = block;
        block = block->next;
    }

    block = request_space(last, aligned_size);
    if (!block) {
        __sync_lock_release(&malloc_lock);
        return NULL;
    }

    if (!heap_base) {
        heap_base = block;
    }

    split_block(block, aligned_size);
    block->magic = MAGIC_USED;

    __sync_lock_release(&malloc_lock);
    return (void *)((char *)block + BLOCK_META_SIZE);
}
