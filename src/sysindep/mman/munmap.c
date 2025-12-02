#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

int munmap(void *addr, size_t length) {
    return (int)syscall2(SYS_munmap, (long)addr, (long)length);
}
