#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

pid_t fork(void) {
	long ret = syscall(SYS_fork);
	if (ret < 0) {
		return -1;
	}
	return (pid_t)ret;
}
