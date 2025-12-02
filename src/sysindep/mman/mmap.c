#include <sys/mman.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdint.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
    long ret = syscall8(
        SYS_mmap, 
        (long)addr, 
        (long)length, 
        (long)prot, 
        (long)flags, 
        (long)fd, 
        0,
        (long)(offset),
        (long)(offset >> 32)
    );

    if (ret < 0) {
         return MAP_FAILED;
    }
    
    return (void *)ret;
}
