#include <sys/mman.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <stdint.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	long ret = syscall(SYS_mmap, addr, length, prot, flags, fd, offset);
	if (ret < 0) {
		return MAP_FAILED;
	}
	return (void *)ret;
}
