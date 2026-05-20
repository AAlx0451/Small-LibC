#include <sys/syscall.h>
#include <unistd.h>

int unlink(const char *path) { return (int)syscall(SYS_unlink, (long)path); }
