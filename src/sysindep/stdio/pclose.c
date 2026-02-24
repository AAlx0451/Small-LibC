#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

struct pid_node {
    int fd;
    pid_t pid;
    struct pid_node *next;
};

extern struct pid_node *__popen_pid_list;
extern volatile int __popen_lock;

int pclose(FILE *stream) {
    int fd, status;
    pid_t pid = 0;
    struct pid_node **pp, *entry;
    pid_t wpid;

    if(stream == NULL) {
        errno = EINVAL;
        return -1;
    }

    fd = fileno(stream);

    _spin_lock(&__popen_lock);

    pp = &__popen_pid_list;
    while(*pp) {
        if((*pp)->fd == fd) {
            entry = *pp;
            *pp = entry->next;
            pid = entry->pid;
            free(entry);
            break;
        }
        pp = &(*pp)->next;
    }

    _spin_unlock(&__popen_lock);

    if(pid == 0) {
        errno = EBADF;
        return -1;
    }

    fclose(stream);

    do {
        wpid = waitpid(pid, &status, 0);
    } while(wpid == -1 && errno == EINTR);

    return (wpid == -1) ? -1 : status;
}
