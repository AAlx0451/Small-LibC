#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

int shm_open(const char *name, int oflag, mode_t mode)
{
    return (int)syscall(SYS_shm_open, (long)name, (long)oflag, (long)mode);
}
