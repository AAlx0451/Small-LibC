#ifndef STDIO_H
#define STDIO_H

#define EOF (-1)
#include <stdarg.h>
#include <unistd.h>

int puts(const char *str);
int putchar(int c);
int _putchar(int c);
int printf(const char* format, ...);
int sprintf(char* buffer, const char* format, ...);
int snprintf(char* buffer, size_t count, const char* format, ...);
int vsnprintf(char* buffer, size_t count, const char* format, va_list va);
int vprintf(const char* format, va_list va);

#endif // STDIO_H
