#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

static inline char* itoa(int n, char* s) { char*p=s,t; unsigned u=n<0?-n:n; do{*p++=u%10+'0';}while(u/=10); if(n<0)*p++='-'; *p='\0'; for(char*e=s;e<--p;e++){t=*e;*e=*p;*p=t;} return s; }

int main()
{
	write(1, "Testing unistd\n\n", 16);
	const char *filename = "./1.txt";
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pid_t pid = fork();
	if (pid == 0) 
	{
		const char *message = "fork... PASS\nwait... PASS\nopen... PASS\nwrite... PASS\n";
		write(fd, message, strlen(message));
		char *cat_argv[] = {"/bin/cat", "1.txt", NULL};
		char *cat_envp[] = {NULL};
		execve("/bin/cat", cat_argv, cat_envp);
	} else {
		wait(0);
		close(fd);
	}
	pid_t id = getpid();
	char txt[20];
	itoa(id, txt);
	char *msg = "\nCurrent process id is ";
	write(1, msg, strlen(msg));
	write(1, txt, strlen(txt));
	write(1, "\n\n", 2);
	write(1, "put one char\t", 13);
	char a[1];
	read(0, a, 1);
	write(1, "\n", 1);
	write(1, a, 1);
	char *ext = "\n\ncheck exit code for _exit()\n";
	write(1, ext, strlen(ext));
	_exit(42);
}
