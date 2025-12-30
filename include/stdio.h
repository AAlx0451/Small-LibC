#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>
#include <sys/types.h>
#include <stdarg.h>

#define BUFSIZ 1024
#define EOF (-1)

#ifndef NULL
#define NULL ((void*)0)
#endif

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define L_tmpnam 1024
#define TMP_MAX 10000

#define FOPEN_MAX 20
#define FILENAME_MAX 1024

typedef off_t fpos_t;

typedef struct __sFILE {
    int _fd;
    unsigned char *_base;
    unsigned char *_ptr;
    size_t _bsize;
    size_t _cnt;
    unsigned int _flags;
    volatile int _lock; // Marked volatile to prevent compiler reordering around locks
    struct __sFILE *_next;
} FILE;

#define __S_RD      0x0001
#define __S_WR      0x0002
#define __S_RW      0x0004
#define __S_ERR     0x0008
#define __S_EOF     0x0010
#define __S_DIRTY   0x0020
#define __S_FREEBUF 0x0040
#define __S_NBF     0x0080
#define __S_STR     0x0100
#define __S_LBF     0x0200
#define __S_RESERVE 0x0400 /* Flag to indicate buffer has a reserve byte for ungetc */

extern FILE *stdin, *stdout, *stderr;

int fflush(FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
void perror(const char *s);
int fgetc(FILE *f);
int fputc(int c, FILE *f);
int vfprintf(FILE *stream, const char *format, va_list arg);
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int vprintf(const char *format, va_list ap);
int sprintf(char *str, const char *format, ...);
int vsprintf(char *str, const char *format, va_list ap);
int snprintf(char *str, size_t size, const char *format, ...);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
FILE *fopen(const char *pathname, const char *mode);
int fclose(FILE *f);
int getchar(void);
int putchar(int c);
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
int ungetc(int c, FILE *f);
int putc(int c, FILE *f);
int getc(FILE *f);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int fseek(FILE *stream, long offset, int whence);
int fseeko(FILE *stream, off_t offset, int whence);
long ftell(FILE *stream);
off_t ftello(FILE *stream);
void rewind(FILE *stream);
char *fgets(char *s, int size, FILE *stream);
char *gets(char *s);
int fputs(const char *s, FILE *stream);
int puts(const char *s);
int fgetpos(FILE *stream, fpos_t *pos);
int fsetpos(FILE *stream, const fpos_t *pos);
void setbuf(FILE *restrict stream, char *restrict buf);
int rename(const char *oldpath, const char *newpath);
int remove(const char *pathname);
int vfscanf(FILE *stream, const char *format, va_list arg);
int scanf(const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int vscanf(const char *format, va_list arg);
int vsscanf(const char *str, const char *format, va_list arg);
int sscanf(const char *str, const char *format, ...);
int dprintf(int fd, const char *format, ...);
int vasprintf(char **strp, const char *fmt, va_list ap);
int asprintf(char **strp, const char *fmt, ...);
FILE *freopen(const char *pathname, const char *mode, FILE *stream);
FILE *tmpfile(void);
char *tmpnam(char *s);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#define feof(p) ((p)->_flags & __S_EOF)
#define ferror(p) ((p)->_flags & __S_ERR)
#define fileno(p) ((p)->_fd)
#define clearerr(p) ((void)((p)->_flags &= ~(__S_ERR | __S_EOF)))


/* Internal API */
#define SMALL_LIBC
void __stdio_init(void);
int __stdio_flush_impl(FILE *stream);
int __stdio_fill_impl(FILE *stream);
void __stdio_add_file(FILE *f);
void __stdio_remove_file(FILE *f);
void __stdio_flush_all(void);
void __stdio_free_buffer(FILE *f);
void _spin_lock(volatile int *lock);
void _spin_unlock(volatile int *lock);

#endif
