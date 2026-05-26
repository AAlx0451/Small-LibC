#ifndef WCHAR_H
#define WCHAR_H

#include <features.h>

#if !defined(_ANSI) && ((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) ||                \
                        (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600) ||                        \
                        (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409LL))

# include <_types.h>
# include <limits.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <time.h>

typedef int wint_t;

# define WEOF ((wint_t) - 1)

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

double wcstod(const wchar_t *restrict nptr, wchar_t **restrict endptr);
long wcstol(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);
unsigned long long wcstoull(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);
unsigned long wcstoul(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);

int fwide(FILE *f, int mode);

wint_t fgetwc(FILE *f);
wint_t getwc(FILE *stream);
wint_t getwchar(void);

wint_t ungetwc(wint_t c, FILE *f);

wint_t fputwc(wchar_t c, FILE *f);
wint_t putwc(wchar_t c, FILE *stream);
wint_t putwchar(wchar_t c);

int fputws(const wchar_t *restrict ws, FILE *restrict stream);
wchar_t *fgetws(wchar_t *restrict ws, int n, FILE *restrict stream);

int wprintf(const wchar_t *format, ...);
int fwprintf(FILE *stream, const wchar_t *format, ...);
int swprintf(wchar_t *restrict s, size_t n, const wchar_t *restrict format, ...);

int vfwprintf(FILE *restrict stream, const wchar_t *restrict format, va_list arg);
int vwprintf(const wchar_t *format, va_list arg);
int vswprintf(wchar_t *restrict ws, size_t n, const wchar_t *restrict format, va_list ap);

int fwscanf(FILE *stream, const wchar_t *format, ...);
int wscanf(const wchar_t *format, ...);
int swscanf(const wchar_t *s, const wchar_t *format, ...);

size_t wcsftime(wchar_t *restrict s,
                size_t maxsize,
                const wchar_t *restrict format,
                const struct tm *restrict timeptr);

# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L

int vfwscanf(FILE *restrict stream, const wchar_t *restrict format, va_list arg);
int vwscanf(const wchar_t *restrict format, va_list arg);
int vswscanf(const wchar_t *restrict ws, const wchar_t *restrict format, va_list arg);

long double wcstold(const wchar_t *restrict nptr, wchar_t **restrict endptr);
float wcstof(const wchar_t *restrict nptr, wchar_t **restrict endptr);

long long wcstoll(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);
unsigned long long wcstoull(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);

# endif /* __STDC_VERSION__ && __STDC_VERSION__ >= 199901L */

#endif /* !_ANSI && ((_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || _XOPEN_SOURCE &&           \
          _XOPEN_SOURCE >= 600) || (__STDC_VERSION__ && __STDC_VERSION__ >= 199409L)) */

#endif /* !WCHAR_H */
