#include "runtime.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uintptr_t __stack_chk_guard;

__attribute__((noreturn)) void __stack_chk_fail(void) {
    const char msg[] = "*** stack smashing detected ***\n";
    write(STDERR_FILENO, msg, sizeof(msg) - 1);
    abort();
}

void __stack_chk_guard_init(void) {
    int fd = open("/dev/urandom", O_RDONLY);
    ssize_t bytes_read;
    if(fd == -1) {
        perror("Could not open /dev/urandom");
        abort();
    }

    bytes_read = read(fd, &__stack_chk_guard, sizeof(__stack_chk_guard));
    close(fd);

    if(bytes_read != sizeof(__stack_chk_guard)) {
        abort();
    }

    ((unsigned char *)&__stack_chk_guard)[0] = 0;
}
