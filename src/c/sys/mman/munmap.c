#include <sys/mman.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <stdint.h>

int munmap(void *addr, size_t length) {
	long ret = syscall(SYS_munmap, addr, length);
        if (ret < 0) {
		return -1;
	}
	return 0;
}
