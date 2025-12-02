#include "malloc.h"

void *realloc(void *ptr, size_t size)
{
    meta_ptr block;
    void *new_ptr;
    size_t aligned_size;

    if (!ptr) {
        return malloc(size);
    }

    if (size == 0) {
        free(ptr);
        return NULL;
    }

    block = get_block_ptr(ptr);
    if (!is_valid_block(block)) {
        return NULL;
    }

    aligned_size = ALIGN(size);

    if (block->size >= aligned_size) {
        if (block->size >= aligned_size + BLOCK_META_SIZE + ALIGNMENT) {
             split_block(block, aligned_size);
             coalesce(block->next); 
        }
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
                return ptr;
            }
        }
    }

    new_ptr = malloc(aligned_size);
    if (!new_ptr) {
        return NULL;
    }

    memcpy(new_ptr, ptr, block->size);
    free(ptr);

    return new_ptr;
}
