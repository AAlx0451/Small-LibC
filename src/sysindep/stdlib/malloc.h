#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sched.h>
#pragma clang diagnostic ignored "-Wreserved-identifier"

#define ALIGNMENT 16U
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define BLOCK_META_SIZE ALIGN(sizeof(struct block_meta))
#define PAGE_SIZE 4096U

#define MAGIC_FREE 0xDEADBEEF
#define MAGIC_USED 0xBA5EBA11

struct block_meta {
    size_t size;
    struct block_meta *next;
    struct block_meta *prev;
    uint32_t magic;
    uint32_t __padding;
};

typedef struct block_meta *meta_ptr;

extern meta_ptr heap_base;
extern volatile int malloc_lock;

void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size);

static inline void __malloc_spin_lock(volatile int *lock) {
    while (__atomic_test_and_set(lock, __ATOMIC_ACQUIRE)) {
        sched_yield();
    }
}

static inline void __malloc_spin_unlock(volatile int *lock) {
    __atomic_clear(lock, __ATOMIC_RELEASE);
}

static inline meta_ptr get_block_ptr(void *ptr) {
    return (meta_ptr)((uintptr_t)ptr - BLOCK_META_SIZE);
}

static inline int is_valid_block(meta_ptr block) {
    if (!block) return 0;
    return (block->magic == MAGIC_USED || block->magic == MAGIC_FREE);
}

static inline void split_block(meta_ptr block, size_t size) {
    if (block->size >= size + BLOCK_META_SIZE + ALIGNMENT) {
        meta_ptr new_block = (meta_ptr)((uintptr_t)block + BLOCK_META_SIZE + size);
        
        new_block->size = block->size - size - BLOCK_META_SIZE;
        new_block->next = block->next;
        new_block->prev = block;
        new_block->magic = MAGIC_FREE;

        block->size = size;
        block->next = new_block;

        if (new_block->next) {
            new_block->next->prev = new_block;
        }
    }
}

static inline meta_ptr coalesce(meta_ptr block) {
    if (block->next && block->next->magic == MAGIC_FREE) {
        if ((char *)block + BLOCK_META_SIZE + block->size == (char *)block->next) {
            block->size += BLOCK_META_SIZE + block->next->size;
            block->next = block->next->next;
            if (block->next) {
                block->next->prev = block;
            }
        }
    }

    if (block->prev && block->prev->magic == MAGIC_FREE) {
        if ((char *)block->prev + BLOCK_META_SIZE + block->prev->size == (char *)block) {
            meta_ptr prev = block->prev;
            prev->size += BLOCK_META_SIZE + block->size;
            prev->next = block->next;
            if (prev->next) {
                prev->next->prev = prev;
            }
            block = prev;
        }
    }
    return block;
}

static inline meta_ptr request_space(meta_ptr last, size_t size) {
    meta_ptr block;
    size_t total_size; 
    size_t alloc_size;

    if (__builtin_add_overflow(size, BLOCK_META_SIZE, &total_size)) {
        return NULL;
    }
    
    alloc_size = (total_size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    
    if (alloc_size < total_size) {
        return NULL;
    }

    block = (meta_ptr)mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (block == MAP_FAILED) {
        return NULL;
    }

    block->size = alloc_size - BLOCK_META_SIZE;
    block->next = NULL;
    block->prev = last;
    block->magic = MAGIC_USED;

    if (last) {
        last->next = block;
    }

    return block;
}

#endif
