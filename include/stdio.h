#ifndef STDIO_H
#define STDIO_H

#include <features.h>
#include <stddef.h>
#include <stdarg.h>

#define BUFSIZ 1024
#define EOF (-1)

#ifndef NULL
#define NULL ((void*)0)
#endif /* !NULL */

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

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
#  define L_ctermid 10
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _POSIX_C_SOURCE || _XOPEN_SOURCE) */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_XOPEN_SOURCE))
#  define L_cuserid 17
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _XOPEN_SOURCE) */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
#include <sys/types.h>
typedef off_t fpos_t;
#else
typedef long long fpos_t;
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _POSIX_C_SOURCE || _XOPEN_SOURCE) */

typedef struct __sFILE {
    int _fd;
    unsigned char *_base;
    unsigned char *_ptr;
    size_t _bsize;
    size_t _cnt;
    unsigned int _flags;
    volatile int _lock;
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
#define __S_RESERVE 0x0400

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
long ftell(FILE *stream);
void rewind(FILE *stream);
char *fgets(char *s, int size, FILE *stream);
int fputs(const char *s, FILE *stream);
int puts(const char *s);
int fgetpos(FILE *stream, fpos_t *pos);
int fsetpos(FILE *stream, const fpos_t *pos);
void setbuf(FILE *restrict stream, char *restrict buf);
int rename(const char *oldpath, const char *newpath);
int remove(const char *pathname);
int scanf(const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int sscanf(const char *str, const char *format, ...);
FILE *freopen(const char *pathname, const char *mode, FILE *stream);
FILE *tmpfile(void);
char *tmpnam(char *s);
int feof(FILE *p);
int ferror(FILE *p);
void clearerr(FILE *p);

#if !(!defined(_ANSI) && (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L))
char *gets(char *s);
#endif /* !(!_ANSI && (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200809L)) */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600))
int snprintf(char *str, size_t size, const char *format, ...);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
int vfscanf(FILE *stream, const char *format, va_list arg);
int vscanf(const char *format, va_list arg);
int vsscanf(const char *str, const char *format, va_list arg);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || (__STDC_VERSION__ && __STDC_VERSION__ >= 199901L) || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600)) */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
int fileno(FILE *p);
FILE *fdopen(int fildes, const char *mode);
char *ctermid(char *s);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _POSIX_C_SOURCE || _XOPEN_SOURCE) */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_XOPEN_SOURCE))
char *cuserid(char *s);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _XOPEN_SOURCE) */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600))
int fseeko(FILE *stream, off_t offset, int whence);
off_t ftello(FILE *stream);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600)) */ 

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700))
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
int dprintf(int fd, const char *format, ...);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200809L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 700)) */

#if (defined(_DARWIN_C_SOURCE) || defined(_GNU_SOURCE)) && !defined(_ANSI)
int vasprintf(char **strp, const char *fmt, va_list ap);
int asprintf(char **strp, const char *fmt, ...);
#endif /* (_DARWIN_C_SOURCE || _GNU_SOURCE) && !_ANSI */

#if defined(_DARWIN_C_SOURCE)
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
#endif /* _DARWIN_C_SOURCE */

#endif /* !STDIO_H */
