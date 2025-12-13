#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

extern void _spin_unlock(volatile int *lock);
extern void _spin_lock(volatile int *lock);

int fclose(FILE *f) {
    if (!f) {
        errno = EINVAL;
        return EOF;
    }

    // Remove from global list BEFORE closing/freeing.
    // This prevents __stdio_flush_all from accessing 'f' after we free it.
    __stdio_remove_file(f);

    int flush_res = 0;
    _spin_lock(&f->_lock);
    
    // Ensure we check flag correctly inside locked region (though f is removed)
    if (f->_flags & __S_WR) {
        flush_res = __stdio_flush_impl(f);
    }
    _spin_unlock(&f->_lock);

    int close_res = close(f->_fd);

    // Use centralized buffer freeing (handles __S_FREEBUF check)
    __stdio_free_buffer(f);

    free(f);

    if (flush_res == EOF || close_res < 0) {
        return EOF;
    }

    return 0;
}
