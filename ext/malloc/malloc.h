#include <unistd.h>
#define NULL 0
#define META_BLOCK_SIZE 20
#include <stdlib.h>

#define allign4(x) (((((x)-1) >> 2) << 2) + 4)
#define align4(x) (((((x)-1) >> 2) << 2) + 4)

typedef struct meta_block *meta_ptr;

void *base = NULL;

struct meta_block                                                                               {
    int free;                                                                                       size_t size;                                                                                    meta_ptr next;                                                                                  meta_ptr prev;                                                                                  void *ptr;                                                                                      char data[1];
};

void free(void *ptr);
int is_addr_valid(void *p); // Скорее всего, этой строки не хватает
meta_ptr get_block_addr(void *p); // И этой
void split_space(meta_ptr block, size_t size); // И этой
meta_ptr merge_blocks(meta_ptr block);
void* memcpy(void* dest, const void* src, unsigned long count);
void *memset(void *ptr, int value, size_t num);
