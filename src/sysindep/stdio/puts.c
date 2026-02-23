#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifndef BUFSIZ
#define BUFSIZ 1024
#endif

int puts(const char *s) {
    int result = 0;
    _spin_lock(&stdout->_lock);

    if(stdout->_base == NULL) {
        size_t size = (stdout->_flags & __S_NBF) ? 1 : BUFSIZ;
        stdout->_base = (unsigned char *)malloc(size);
        if(stdout->_base == NULL) {
            result = EOF;
            goto done;
        }
        stdout->_flags |= __S_FREEBUF;
        stdout->_bsize = size;
        stdout->_ptr = stdout->_base;
        stdout->_cnt = size;
    }

    if(stdout->_flags & __S_RD) {
        stdout->_flags &= ~__S_RD;
        stdout->_flags |= __S_WR;
        stdout->_cnt = stdout->_bsize;
        stdout->_ptr = stdout->_base;
    } else if(!(stdout->_flags & __S_WR)) {
        stdout->_flags |= __S_WR;
        if(stdout->_cnt == 0 && stdout->_ptr == stdout->_base) {
            stdout->_cnt = stdout->_bsize;
        }
    }

    stdout->_flags |= __S_DIRTY;

    while(*s) {
        if(stdout->_cnt <= 0) {
            if(__stdio_flush_impl(stdout) == EOF) {
                result = EOF;
                goto done;
            }
            stdout->_cnt = stdout->_bsize;
            stdout->_ptr = stdout->_base;
        }
        *stdout->_ptr++ = *s++;
        stdout->_cnt--;
    }

    if(stdout->_cnt <= 0) {
        if(__stdio_flush_impl(stdout) == EOF) {
            result = EOF;
            goto done;
        }
        stdout->_cnt = stdout->_bsize;
        stdout->_ptr = stdout->_base;
    }
    *stdout->_ptr++ = '\n';
    stdout->_cnt--;

    if((stdout->_flags & __S_LBF) || (stdout->_flags & __S_NBF)) {
        if(__stdio_flush_impl(stdout) == EOF) {
            result = EOF;
        }
    }

done:
    _spin_unlock(&stdout->_lock);
    return (result == EOF) ? EOF : 0;
}
