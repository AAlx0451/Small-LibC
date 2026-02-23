#ifndef STRING_H
#define STRING_H

#include <features.h>
#include <stddef.h>

#ifndef MEMORY_H
#define MEMORY_H

void *memchr(const void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

#endif /* !MEMORY_H */

void *memmove(void *dest, const void *src, size_t count);

char *strcat(char *dest, const char *src);
char *strcpy(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
char *strtok(char *s, const char *delim);
size_t strxfrm(char *dest, const char *src, size_t n);

char *strchr(const char *s, int c);
int strcmp(const char *s1, const char *s2);
int strcoll(const char *s1, const char *s2);
size_t strcspn(const char *s, const char *reject);
char *strerror(int errnum);
size_t strlen(const char* s);
int strncmp(const char *s1, const char *s2, size_t n);
char *strpbrk(const char *s, const char *accept);
char *strrchr(const char *s, int c);
size_t strspn(const char *s, const char *accept);
char *strstr(const char *haystack, const char *needle);

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500) || \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L))
char *strdup(const char *s);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 500) || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200809L)) */

#endif /* !STRING_H */
