#include "malloc.h"

meta_ptr merge_blocks(meta_ptr block)
{
    if (block->next && block->next->free) {
        block->size += BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }
    return block;
}

meta_ptr get_block_addr(void *p)
{
    return (meta_ptr)p - 1;
}

int is_addr_valid(void *p)
{
    if (base) {
        if (p > base && p < sbrk(0)) {
            return 1;
        }
    }
    return 0;
}

void free(void *ptr)
{
    meta_ptr block;

    if (!is_addr_valid(ptr)) {
        return;
    }

    block = get_block_addr(ptr);
    block->free = 1;

    if (block->prev && block->prev->free) {
        block = merge_blocks(block->prev);
    }

    if (block->next) {
        merge_blocks(block);
    } else {
        if (block->prev) {
            block->prev->next = NULL;
        } else {
            base = NULL;
        }
        brk(block);
    }
}
