#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value) {
    return (int)syscall(SYS_setitimer,
                        (long)which,
                        (long)new_value,
                        (long)old_value);
}
