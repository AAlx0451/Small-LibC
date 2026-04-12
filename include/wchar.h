#ifndef WCHAR_H
#define WCHAR_H

#include <features.h>

#if !defined(_ANSI) && ((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409LL))

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>

typedef int wint_t;

typedef struct
{
    int __count;
    char __bytes[4];
} mbstate_t;

#define WEOF ((wint_t)-1)

wchar_t *wcscat(wchar_t *dest, const wchar_t *src);
wchar_t *wcsncat(wchar_t *dest, const wchar_t *src, size_t n);
wchar_t *wcschr(const wchar_t *s, wchar_t c);
wchar_t *wcsrchr(const wchar_t *s, wchar_t c);
int wcscmp(const wchar_t *s1, const wchar_t *s2);
int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wcscpy(wchar_t *dest, const wchar_t *src);
wchar_t *wcsncpy(wchar_t *dest, const wchar_t *src, size_t n);
size_t wcscspn(const wchar_t *s, const wchar_t *reject);
size_t wcslen(const wchar_t *s);
wchar_t *wcspbrk(const wchar_t *s, const wchar_t *accept);
size_t wcsspn(const wchar_t *s, const wchar_t *accept);
wchar_t *wcsstr(const wchar_t *haystack, const wchar_t *needle);
wchar_t *wcstok(wchar_t *str, const wchar_t *delim, wchar_t **ptr);
wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n);
int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wmemcpy(wchar_t *dest, const wchar_t *src, size_t n);
wchar_t *wmemmove(wchar_t *dest, const wchar_t *src, size_t n);
wchar_t *wmemset(wchar_t *s, wchar_t c, size_t n);
int wcscoll(const wchar_t *s1, const wchar_t *s2);
size_t wcsxfrm(wchar_t *dest, const wchar_t *src, size_t n);

int mbsinit(const mbstate_t *ps);
size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps);
size_t mbrlen(const char *s, size_t n, mbstate_t *ps);
size_t wcrtomb(char *s, wchar_t wc, mbstate_t *ps);
size_t mbsrtowcs(wchar_t *dst, const char **src, size_t len, mbstate_t *ps);
size_t wcsrtombs(char *dst, const wchar_t **src, size_t len, mbstate_t *ps);

int fwide(FILE *f, int mode);

#endif /* !_ANSI && ((_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || _XOPEN_SOURCE && _XOPEN_SOURCE >= 600) || (__STDC_VERSION__ && __STDC_VERSION__ >= 199409L)) */

#endif /* !WCHAR_H */
