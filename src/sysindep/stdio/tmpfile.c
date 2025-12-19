#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

FILE *tmpfile(void) {
    char path[] = "/tmp/tmp.XXXXXX";
    int fd;
    FILE *f;
    unsigned char *buf;

    /* create a secure temporary file using mkstemp */
    fd = mkstemp(path);
    if (fd == -1) {
        return NULL;
    }

    /* unlink the file immediately */
    if (unlink(path) == -1) {
        close(fd);
        return NULL;
    }

    /* allocate FILE object */
    f = (FILE *)malloc(sizeof(FILE));
    if (f == NULL) {
        close(fd);
        return NULL;
    }

    buf = (unsigned char *)malloc(BUFSIZ + 1);
    if (buf == NULL) {
        free(f);
        close(fd);
        return NULL;
    }

    f->_fd = fd;
    f->_base = buf + 1;
    f->_ptr = f->_base;
    f->_bsize = BUFSIZ;
    f->_cnt = 0;
    f->_lock = 0;
    f->_next = NULL;
    f->_flags = __S_RW | __S_FREEBUF | __S_RESERVE;

    /* register the file in the global list */
    __stdio_add_file(f);

    return f;
}
