#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

int shm_unlink(const char *name) { return (int)syscall(SYS_shm_unlink, (long)name); }
