#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

extern char **environ;

int system(const char *command) {
	pid_t pid;
	int status;
	char *argv[4];

	if (command == NULL) {
		return 1; 
	}

	pid = fork();
	if (pid == -1) {
		return -1;
	}

	if (pid == 0) {
		argv[0] = "sh";
		argv[1] = "-c";
		argv[2] = (char *)command;
		argv[3] = NULL;
		
		execve("/bin/sh", argv, environ);
		_exit(127);
	}

	if (waitpid(pid, &status, 0) == -1) {
		return -1;
	}

	return status;
}
