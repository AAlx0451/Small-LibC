#include "malloc.h"

void *realloc(void *ptr, size_t size)
{
    meta_ptr block;
    void *new_ptr;
    size_t aligned_size;
    size_t copy_size;

    if (!ptr) {
        return malloc(size);
    }

    if (size == 0) {
        free(ptr);
        return NULL;
    }

    aligned_size = ALIGN(size);

    while (__sync_lock_test_and_set(&malloc_lock, 1)) {
        while (malloc_lock);
    }

    block = get_block_ptr(ptr);
    if (!is_valid_block(block)) {
        __sync_lock_release(&malloc_lock);
        return NULL;
    }

    if (block->size >= aligned_size) {
        if (block->size >= aligned_size + BLOCK_META_SIZE + ALIGNMENT) {
             split_block(block, aligned_size);
             coalesce(block->next); 
        }
        __sync_lock_release(&malloc_lock);
        return ptr;
    }

    if (block->next && block->next->magic == MAGIC_FREE) {
        if ((char *)block + BLOCK_META_SIZE + block->size == (char *)block->next) {
            size_t combined_size = block->size + BLOCK_META_SIZE + block->next->size;
            if (combined_size >= aligned_size) {
                block->magic = MAGIC_FREE;
                block = coalesce(block);
                block->magic = MAGIC_USED;
                split_block(block, aligned_size);
                __sync_lock_release(&malloc_lock);
                return ptr;
            }
        }
    }

    __sync_lock_release(&malloc_lock);

    new_ptr = malloc(size);
    if (!new_ptr) {
        return NULL;
    }

    copy_size = block->size < aligned_size ? block->size : aligned_size;
    memcpy(new_ptr, ptr, copy_size);
    free(ptr);

    return new_ptr;
}
