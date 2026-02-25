#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

struct pid_node {
    int fd;
    pid_t pid;
    struct pid_node *next;
};

extern volatile int __popen_lock;
extern struct pid_node *__popen_pid_list;
struct pid_node *__popen_pid_list = NULL;
volatile int __popen_lock = 0;

FILE *popen(const char *command, const char *mode) {
    int p_h[2];
    pid_t pid;
    FILE *fp;
    int parent_side, child_side, target_fd;
    struct pid_node *new_node, *cur;

    if((mode[0] != 'r' && mode[0] != 'w') || mode[1] != 0) {
        errno = EINVAL;
        return NULL;
    }

    if(pipe(p_h) == -1) {
        return NULL;
    }

    if(mode[0] == 'r') {
        parent_side = p_h[0];
        child_side = p_h[1];
        target_fd = STDOUT_FILENO;
    } else {
        parent_side = p_h[1];
        child_side = p_h[0];
        target_fd = STDIN_FILENO;
    }

    _spin_lock(&__popen_lock);

    if((pid = fork()) == -1) {
        _spin_unlock(&__popen_lock);
        close(p_h[0]);
        close(p_h[1]);
        return NULL;
    }

    if(pid == 0) {
        cur = __popen_pid_list;
        while(cur) {
            close(cur->fd);
            cur = cur->next;
        }

        close(parent_side);

        if(child_side != target_fd) {
            dup2(child_side, target_fd);
            close(child_side);
        }

        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        _exit(127);
    }

    close(child_side);

    if((fp = fdopen(parent_side, mode)) == NULL) {
        close(parent_side);
        return NULL;
    }

    fcntl(parent_side, F_SETFD, FD_CLOEXEC);

    new_node = (struct pid_node *)malloc(sizeof(struct pid_node));
    if(!new_node) {
        fclose(fp);
        _spin_unlock(&__popen_lock);
        return NULL;
    }

    new_node->fd = fileno(fp);
    new_node->pid = pid;
    new_node->next = __popen_pid_list;
    __popen_pid_list = new_node;

    _spin_unlock(&__popen_lock);

    return fp;
}
