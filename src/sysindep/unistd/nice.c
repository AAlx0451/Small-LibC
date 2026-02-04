#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

int nice(int incr) {
    int current_prio;
    int new_prio;
   
    errno = 0;
    current_prio = syscall(SYS_getpriority, 0, 0); 
    
    if (current_prio == -1 && errno != 0) {
        return -1;
    }

    new_prio = current_prio + incr;
    if (new_prio < -20) new_prio = -20;
    if (new_prio > 20)  new_prio = 20;

    if (syscall(SYS_setpriority, 0, 0, new_prio) == -1) {
        return -1;
    }

    return new_prio;
}
