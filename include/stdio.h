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

typedef struct __sFILE {
    int _fd;
    unsigned char *_base;
    unsigned char *_ptr;
    size_t _bsize;
    size_t _cnt;
    unsigned int _flags;
    int _err;
    int _lock; 
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

#define feof(p) ((p)->_flags & __S_EOF)
#define ferror(p) ((p)->_flags & __S_ERR)
#define fileno(p) ((p)->_fd)

void __stdio_init(void);
int __stdio_flush_impl(FILE *stream);
int __stdio_fill_impl(FILE *stream);
void __stdio_add_file(FILE *f);
void __stdio_remove_file(FILE *f);
void __stdio_flush_all(void);

#endif
