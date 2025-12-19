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

    if (((uintptr_t)ptr & (ALIGNMENT - 1)) != 0) {
        return NULL;
    }

    if (size > (SIZE_MAX - BLOCK_META_SIZE)) {
        return NULL;
    }

    aligned_size = ALIGN(size);

    __malloc_spin_lock(&malloc_lock);

    block = get_block_ptr(ptr);
    if (!is_valid_block(block) || block->magic != MAGIC_USED) {
        __malloc_spin_unlock(&malloc_lock);
        return NULL;
    }

    if (block->size >= aligned_size) {
        split_block(block, aligned_size);
        __malloc_spin_unlock(&malloc_lock);
        return ptr;
    }

    if (block->next && block->next->magic == MAGIC_FREE) {
        if ((char *)block + BLOCK_META_SIZE + block->size == (char *)block->next) {
            size_t combined_size;
            if (!__builtin_add_overflow(block->size, BLOCK_META_SIZE, &combined_size) &&
                !__builtin_add_overflow(combined_size, block->next->size, &combined_size)) {
                
                if (combined_size >= aligned_size) {
                    block->size += BLOCK_META_SIZE + block->next->size;
                    block->next = block->next->next;
                    if (block->next) {
                        block->next->prev = block;
                    }
                    
                    split_block(block, aligned_size);
                    __malloc_spin_unlock(&malloc_lock);
                    return ptr;
                }
            }
        }
    }

    __malloc_spin_unlock(&malloc_lock);

    new_ptr = malloc(size);
    if (!new_ptr) {
        return NULL;
    }

    copy_size = block->size < size ? block->size : size;
    memcpy(new_ptr, ptr, copy_size);
    free(ptr);

    return new_ptr;
}
