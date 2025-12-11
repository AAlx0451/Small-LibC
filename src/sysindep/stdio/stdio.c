#include <unistd.h>
#include <errno.h>
#include <stdio.h>

static FILE *__s_file_list_head = NULL;

static FILE _f_stdin;
static FILE _f_stdout;
static FILE _f_stderr;
static unsigned char _buf_stdin[BUFSIZ];
static unsigned char _buf_stdout[BUFSIZ];

FILE *stdin;
FILE *stdout;
FILE *stderr;

void __stdio_add_file(FILE *f) {
    f->_next = __s_file_list_head;
    __s_file_list_head = f;
}

void __stdio_remove_file(FILE *f) {
    if (__s_file_list_head == f) {
        __s_file_list_head = f->_next;
    } else {
        FILE *p = __s_file_list_head;
        while (p && p->_next != f) {
            p = p->_next;
        }
        if (p) {
            p->_next = f->_next;
        }
    }
}

void __stdio_flush_all(void) {
    FILE *p;
    p = __s_file_list_head;
    while (p) {
        if (p->_flags & __S_WR) {
            __stdio_flush_impl(p);
        }
        p = p->_next;
    }
}

int __stdio_flush_impl(FILE *f) {
    if (!f || !f->_base) return 0;

    if ((f->_flags & __S_DIRTY) && (f->_flags & __S_WR)) {
        size_t size = f->_ptr - f->_base;
        if (size > 0) {
            ssize_t written = 0;
            size_t total = 0;
            while (total < size) {
                written = write(f->_fd, f->_base + total, size - total);
                if (written < 0) {
                    if (errno == EINTR) continue;
                    f->_flags |= __S_ERR;
                    return EOF;
                }
                total += written;
            }
        }
        f->_ptr = f->_base;
        f->_cnt = f->_bsize;
        f->_flags &= ~__S_DIRTY;
    }
    return 0;
}

int __stdio_fill_impl(FILE *f) {
    ssize_t n;

    if (f->_flags & (__S_EOF | __S_ERR)) return EOF;

    if ((f->_flags & __S_WR) && (f->_flags & __S_DIRTY)) {
        if (__stdio_flush_impl(f) != 0) return EOF;
    }
    f->_flags &= ~__S_WR;
    f->_flags |= __S_RD;

    n = read(f->_fd, f->_base, f->_bsize);
    if (n <= 0) {
        if (n == 0) f->_flags |= __S_EOF;
        else f->_flags |= __S_ERR;
        f->_cnt = 0;
        return EOF;
    }

    f->_ptr = f->_base;
    f->_cnt = n;
    return 0;
}

void __stdio_init(void) {
    /* STDIN */
    _f_stdin._fd = 0;
    _f_stdin._base = _buf_stdin;
    _f_stdin._ptr = _buf_stdin;
    _f_stdin._bsize = BUFSIZ;
    _f_stdin._cnt = 0;
    _f_stdin._flags = __S_RD;
    stdin = &_f_stdin;

    /* STDOUT */
    _f_stdout._fd = 1;
    _f_stdout._base = _buf_stdout;
    _f_stdout._ptr = _buf_stdout;
    _f_stdout._bsize = BUFSIZ;
    _f_stdout._cnt = BUFSIZ;
    _f_stdout._flags = __S_WR;
    stdout = &_f_stdout;

    /* STDERR */
    _f_stderr._fd = 2;
    _f_stderr._base = NULL;
    _f_stderr._ptr = NULL;
    _f_stderr._bsize = 0;
    _f_stderr._cnt = 0;
    _f_stderr._flags = __S_WR | __S_NBF;
    stderr = &_f_stderr;

    __stdio_add_file(&_f_stderr);
    __stdio_add_file(&_f_stdout);
    __stdio_add_file(&_f_stdin);
}
