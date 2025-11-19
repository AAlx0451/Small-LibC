#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

pid_t waitpid(pid_t pid, int *wstatus, int options) {
	long ret = syscall(SYS_wait4, pid, wstatus, options, NULL);
	if (ret < 0) {
		return -1;
	}
	return (pid_t)ret;
}
