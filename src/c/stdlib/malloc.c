#include "malloc.h"

meta_ptr heap_base = NULL;
volatile int malloc_lock = 0;

void *malloc(size_t size)
{
    meta_ptr block, last;
    size_t aligned_size;

    if (size == 0 || size > (SIZE_MAX - BLOCK_META_SIZE)) {
        return NULL;
    }

    aligned_size = ALIGN(size);

    __malloc_spin_lock(&malloc_lock);

    block = heap_base;
    last = NULL;

    while (block) {
        if (block->magic == MAGIC_FREE && block->size >= aligned_size) {
            block->magic = MAGIC_USED;
            split_block(block, aligned_size);
            __malloc_spin_unlock(&malloc_lock);
            return (void *)((char *)block + BLOCK_META_SIZE);
        }
        last = block;
        block = block->next;
    }

    block = request_space(last, aligned_size);
    if (!block) {
        __malloc_spin_unlock(&malloc_lock);
        return NULL;
    }

    if (!heap_base) {
        heap_base = block;
    }

    split_block(block, aligned_size);
    block->magic = MAGIC_USED;

    __malloc_spin_unlock(&malloc_lock);
    return (void *)((char *)block + BLOCK_META_SIZE);
}
