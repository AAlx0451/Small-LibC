#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

static int sbrk_needs_init = 1;                          /* TRUE */
static const size_t sbrk_total_size = 128 * 1024 * 1024; /* 128MB Fixed Region */
static uint8_t *sbrk_base;
static uint8_t *sbrk_curbrk;

int brk(void *addr);

void *sbrk(intptr_t size) {
    if(sbrk_needs_init) {
        sbrk_needs_init = 0; /* FALSE */
        void *base = mmap(NULL, sbrk_total_size,
                          PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS,
                          -1, 0);

        if(base == MAP_FAILED) {
            return (void *)-1;
        }

        sbrk_base = (uint8_t *)base;
        sbrk_curbrk = (uint8_t *)base;
    }

    if(size == 0) {
        return (void *)sbrk_curbrk;
    }

    void *old_brk = sbrk_curbrk;
    uint8_t *new_brk = sbrk_curbrk + size;

    if(new_brk >= sbrk_base && new_brk <= sbrk_base + sbrk_total_size) {
        sbrk_curbrk = new_brk;
        return old_brk;
    }

    errno = ENOMEM;
    return (void *)-1;
}

int brk(void *addr) {
    if(sbrk_needs_init) {
        if(sbrk(0) == (void *)-1) {
            return -1;
        }
    }

    uint8_t *new_brk = (uint8_t *)addr;
    if(new_brk >= sbrk_base && new_brk <= sbrk_base + sbrk_total_size) {
        sbrk_curbrk = new_brk;
        return 0;
    }

    errno = ENOMEM;
    return -1;
}
