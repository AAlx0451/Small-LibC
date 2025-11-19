#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ALIGN4(x) (((((x) - 1) >> 2) << 2) + 4)
#define BLOCK_SIZE sizeof(struct meta_block)

typedef struct meta_block *meta_ptr;

struct meta_block {
    size_t size;
    int free;
    struct meta_block *next;
    struct meta_block *prev;
};

extern void *base;

void free(void *ptr);
int is_addr_valid(void *p);
meta_ptr get_block_addr(void *p);
void split_space(meta_ptr block, size_t size);
meta_ptr merge_blocks(meta_ptr block);
void *malloc(size_t size);
void *calloc(size_t number, size_t size);
void *realloc(void *p, size_t size);

#endif
