#include "malloc.h"

void *realloc(void *p, size_t size)
{
    size_t s;
    meta_ptr old_block;
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

    s = ALIGN(size);
    old_block = get_block_addr(p);

    if (old_block->size >= s) {
        if (old_block->size >= s + BLOCK_SIZE + 8) {
            split_space(old_block, s);
        }
        return p;
    }

    if (old_block->next && old_block->next->free) {
        if ((char *)old_block + BLOCK_SIZE + old_block->size == (char *)old_block->next) {
            if ((old_block->size + BLOCK_SIZE + old_block->next->size) >= s) {
                merge_blocks(old_block);
                if (old_block->size >= s + BLOCK_SIZE + 8) {
                    split_space(old_block, s);
                }
                return p;
            }
        }
    }

    new_ptr = malloc(s);
    if (!new_ptr) {
        return NULL;
    }

    if (old_block->size < s) {
        memmove(new_ptr, p, old_block->size);
    } else {
        memmove(new_ptr, p, s);
    }

    free(p);
    return new_ptr;
}
