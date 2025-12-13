#if defined(__APPLE__) && defined(__arm__)

#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>

/*
 * Apple (BSD) pipe implementation.
 * Kernel returns read_fd in r0 and write_fd in r1.
 * Standard syscall() macro cannot handle generic return in r1.
 */
int pipe(int fildes[2])
{
    register long r12_syscall __asm__("r12") = SYS_pipe; 
    long fd0, fd1;
    long err;

    __asm__ volatile (
        "svc #0x80\n\t"
        "bcc 1f\n\t"            // Branch if Carry Clear (Success)
        
        // Error path (Carry Set)
        "mov %[err], r0\n\t"    // r0 contains errno
        "mov %[fd0], #0\n\t"    // dummy cleanup
        "mov %[fd1], #0\n\t"    // dummy cleanup
        "b 2f\n\t"
        
        // Success path
        "1:\n\t"
        "mov %[err], #0\n\t"
        "mov %[fd0], r0\n\t"    // r0 contains fildes[0]
        "mov %[fd1], r1\n\t"    // r1 contains fildes[1]
        
        "2:\n\t"
        : [fd0] "=r" (fd0), 
          [fd1] "=r" (fd1), 
          [err] "=r" (err)
        : "r" (r12_syscall)
        : "r0", "r1", "cc", "memory"
    );

    if (err != 0) {
        errno = (int)err;
        return -1;
    }

    if (fildes) {
        fildes[0] = (int)fd0;
        fildes[1] = (int)fd1;
    }

    return 0;
}

#endif
