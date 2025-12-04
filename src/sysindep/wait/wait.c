#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

pid_t wait(int *wstatus) {
    return wait4(-1, wstatus, 0, NULL);
}
