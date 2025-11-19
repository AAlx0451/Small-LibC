#include "malloc.h"

void *realloc(void *p, size_t size)
{
    size_t s;
    meta_ptr old_block, new_block;
    void *new_ptr;

    if (!p) {
        return malloc(size);
    }

    if (size == 0) {
        free(p);
        return NULL;
    }

    if (!is_addr_valid(p)) {
        return NULL;
    }

    s = ALIGN4(size);
    old_block = get_block_addr(p);

    if (old_block->size >= s) {
        if (old_block->size >= s + BLOCK_SIZE + 4) {
            split_space(old_block, s);
        }
        return p;
    }

    if (old_block->next && old_block->next->free &&
        (old_block->size + BLOCK_SIZE + old_block->next->size) >= s) {
        merge_blocks(old_block);
        if (old_block->size >= s + BLOCK_SIZE + 4) {
            split_space(old_block, s);
        }
        return p;
    }

    new_ptr = malloc(s);
    if (!new_ptr) {
        return NULL;
    }

    new_block = get_block_addr(new_ptr);
    
    if (old_block->size < s) {
        memcpy(new_ptr, p, old_block->size);
    } else {
        memcpy(new_ptr, p, s);
    }

    free(p);
    return new_ptr;
}
