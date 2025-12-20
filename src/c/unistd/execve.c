#include <unistd.h>
#include <sys/syscall.h>

int execve(const char *pathname, char *const argv[], char *const envp[]) {
	long ret = syscall(SYS_execve, pathname, argv, envp);
	if (ret < 0) {
		return -1;
	}
	return 0;
}
