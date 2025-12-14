#include <stdio.h>
#include <sys/types.h> // off_t

int puts(const char *s) {
    int result = 0;

    _spin_lock(&stdout->_lock);
    
    // unlikely for stdout, handle R->W transition.
    if (stdout->_flags & __S_RD) {
        _spin_unlock(&stdout->_lock);
        if (fseeko(stdout, 0, SEEK_CUR) != 0) {
            return EOF;
        }
        _spin_lock(&stdout->_lock);
    }
    
    if (!(stdout->_flags & __S_WR)) {
        stdout->_flags |= __S_WR;
        stdout->_cnt = stdout->_bsize;
        stdout->_ptr = stdout->_base;
    }

    stdout->_flags |= __S_DIRTY;

    while (*s) {
        if (stdout->_cnt == 0) {
            if (__stdio_flush_impl(stdout) == EOF) {
                result = EOF;
                goto done;
            }
            stdout->_cnt = stdout->_bsize;
        }
        *stdout->_ptr++ = *s++;
        stdout->_cnt--;
    }
    
    if (stdout->_cnt == 0) {
        if (__stdio_flush_impl(stdout) == EOF) {
            result = EOF;
            goto done;
        }
        stdout->_cnt = stdout->_bsize;
    }
    *stdout->_ptr++ = '\n';
    stdout->_cnt--;

    if ((stdout->_flags & __S_LBF) || (stdout->_flags & __S_NBF)) {
        if (__stdio_flush_impl(stdout) == EOF) {
            result = EOF;
        }
    }

done:
    _spin_unlock(&stdout->_lock);
    return result;
}
