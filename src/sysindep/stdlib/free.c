#include "malloc.h"

void free(void *ptr)
{
    meta_ptr block;

    if (!ptr) {
        return;
    }

    block = get_block_ptr(ptr);

    if (!is_valid_block(block) || block->magic == MAGIC_FREE) {
        return;
    }

    block->magic = MAGIC_FREE;
    coalesce(block);
}
