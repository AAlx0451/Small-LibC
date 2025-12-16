#include <unistd.h>
#include <sys/syscall.h>

int rmdir(const char *path) {
	return syscall(SYS_rmdir, (long)path);
}
