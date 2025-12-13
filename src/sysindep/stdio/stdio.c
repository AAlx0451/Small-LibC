#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h> // for sched_yield

static FILE *__s_file_list_head = NULL;
static volatile int __stdio_list_lock = 0;

static FILE _f_stdin;
static FILE _f_stdout;
static FILE _f_stderr;

static unsigned char _buf_stdin[BUFSIZ];
static unsigned char _buf_stdout[BUFSIZ];
static unsigned char _buf_stderr[1];

FILE *stdin;
FILE *stdout;
FILE *stderr;

void _spin_lock(volatile int *lock) {
    while (__sync_lock_test_and_set(lock, 1)) {
        while (*lock) {
            // Prevent Livelock by yielding CPU
            sched_yield();
        }
    }
}

void _spin_unlock(volatile int *lock) {
    __sync_lock_release(lock);
}

void __stdio_add_file(FILE *f) {
    _spin_lock(&__stdio_list_lock);
    f->_next = __s_file_list_head;
    __s_file_list_head = f;
    _spin_unlock(&__stdio_list_lock);
}

void __stdio_remove_file(FILE *f) {
    _spin_lock(&__stdio_list_lock);
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
    _spin_unlock(&__stdio_list_lock);
}

// Centralized buffer freeing logic
void __stdio_free_buffer(FILE *f) {
    if (f->_flags & __S_FREEBUF) {
        // If __S_RESERVE is set, the actual pointer returned by malloc was
        // f->_base - 1. See fopen.c
        unsigned char *real_ptr = f->_base;
        if (f->_flags & __S_RESERVE) {
            real_ptr--;
        }
        free(real_ptr);
        f->_flags &= ~(__S_FREEBUF | __S_RESERVE);
        f->_base = NULL;
    }
}

void __stdio_flush_all(void) {
    FILE *p;
    _spin_lock(&__stdio_list_lock);
    p = __s_file_list_head;
    while (p) {
        _spin_lock(&p->_lock);
        if (p->_flags & __S_WR) {
            __stdio_flush_impl(p);
        }
        _spin_unlock(&p->_lock);
        p = p->_next;
    }
    _spin_unlock(&__stdio_list_lock);
}

int __stdio_flush_impl(FILE *f) {
    if (!f || !f->_base) return 0;

    if ((f->_flags & __S_DIRTY) && (f->_flags & __S_WR)) {
        size_t size = f->_ptr - f->_base;
        unsigned char *p = f->_base;

        while (size > 0) {
            ssize_t written = write(f->_fd, p, size);

            if (written < 0) {
                if (errno == EINTR) continue;
                f->_flags |= __S_ERR;
                return EOF;
            }
            if (written == 0) {
                f->_flags |= __S_ERR;
                return EOF;
            }

            p += written;
            size -= written;
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

    do {
        n = read(f->_fd, f->_base, f->_bsize);
    } while (n < 0 && errno == EINTR);

    if (n <= 0) {
        if (n == 0) f->_flags |= __S_EOF;
        else f->_flags |= __S_ERR;
        f->_cnt = 0;
        return EOF;
    }

    f->_ptr = f->_base;
    f->_cnt = (size_t)n;
    return 0;
}

void __stdio_init(void) {
    _f_stdin._fd = 0;
    _f_stdin._base = _buf_stdin;
    _f_stdin._ptr = _buf_stdin;
    _f_stdin._bsize = BUFSIZ;
    _f_stdin._cnt = 0;
    _f_stdin._flags = __S_RD;
    _f_stdin._lock = 0;
    stdin = &_f_stdin;

    _f_stdout._fd = 1;
    _f_stdout._base = _buf_stdout;
    _f_stdout._ptr = _buf_stdout;
    _f_stdout._bsize = BUFSIZ;
    _f_stdout._cnt = BUFSIZ;
    _f_stdout._flags = __S_WR;
    _f_stdout._lock = 0;
    stdout = &_f_stdout;

    _f_stderr._fd = 2;
    _f_stderr._base = _buf_stderr;
    _f_stderr._ptr = _buf_stderr;
    _f_stderr._bsize = 1;
    _f_stderr._cnt = 1;
    _f_stderr._flags = __S_WR | __S_NBF;
    _f_stderr._lock = 0;
    stderr = &_f_stderr;

    if (isatty(fileno(stdin))) {
        setvbuf(stdin, NULL, _IOLBF, BUFSIZ);
    }
    if (isatty(fileno(stdout))) {
        setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
    }

    __stdio_add_file(&_f_stderr);
    __stdio_add_file(&_f_stdout);
    __stdio_add_file(&_f_stdin);
}
