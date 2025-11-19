#include "malloc.h"

void *base = NULL;

meta_ptr find_suitable_block(meta_ptr *last, size_t size)
{
    meta_ptr b = base;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    }
    return b;
}

void split_space(meta_ptr block, size_t size)
{
    meta_ptr new_block;
    
    new_block = (meta_ptr)((char *)block + BLOCK_SIZE + size);
    new_block->size = block->size - size - BLOCK_SIZE;
    new_block->next = block->next;
    new_block->free = 1;
    new_block->prev = block;
    
    block->size = size;
    block->next = new_block;
    
    if (new_block->next) {
        new_block->next->prev = new_block;
    }
}

meta_ptr extend_heap(meta_ptr last, size_t size)
{
    meta_ptr block;
    
    block = sbrk(0);
    if (sbrk(BLOCK_SIZE + size) == (void *)-1) {
        return NULL;
    }
    
    block->size = size;
    block->next = NULL;
    block->prev = last;
    block->free = 0;
    
    if (last) {
        last->next = block;
    }
    return block;
}

void *malloc(size_t size)
{
    meta_ptr block, last;
    size_t s;

    if (size <= 0) {
        return NULL;
    }

    s = ALIGN4(size);

    if (base) {
        last = base;
        block = find_suitable_block(&last, s);
        if (block) {
            if (block->size >= s + BLOCK_SIZE + 4) {
                split_space(block, s);
            }
            block->free = 0;
        } else {
            block = extend_heap(last, s);
            if (!block) {
                return NULL;
            }
        }
    } else {
        block = extend_heap(NULL, s);
        if (!block) {
            return NULL;
        }
        base = block;
    }
    return (void *)(block + 1);
}
