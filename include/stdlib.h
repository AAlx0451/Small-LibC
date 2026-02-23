#ifndef STDLIB_H
#define STDLIB_H

#include <features.h>
#include <stddef.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define RAND_MAX 0x7FFFFFFF
#define MB_CUR_MAX 4

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600))
typedef struct {
    long long quot;
    long long rem;
} lldiv_t;
#endif /* !_ANSI && (_DARWIN_C_SOURCE || (__STDC_VERSION__ && __STDC_VERSION__ >= 199901L) || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600)) */

void free(void *ptr);
void *malloc(size_t size);
void *calloc(size_t number, size_t size);
void *realloc(void *p, size_t size);

NORETURN void exit(int status);

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600))
NORETURN void _Exit(int status);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || (__STDC_VERSION__ && __STDC_VERSION__ >= 199901L) || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600)) */

int abs(int num);
long labs(long num);
div_t div(int numer, int denom);
ldiv_t ldiv(long numer, long denom);
void srand(unsigned int seed);
int rand(void);

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600))
long long llabs(long long j);
lldiv_t lldiv(long long numer, long long denom);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || (__STDC_VERSION__ && __STDC_VERSION__ >= 199901L) || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600)) */

int atoi(const char *nptr);
long atol(const char *nptr);
double atof(const char *str);
double strtod(const char *str, char **end);
long strtol(const char *restrict nptr, char **restrict endptr, int base);
unsigned long strtoul(const char *restrict nptr, char **restrict endptr, int base);

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600))
long long atoll(const char *nptr);
long double strtold(const char *restrict nptr, char **restrict endptr);
float strtof(const char *restrict nptr, char **restrict endptr);
unsigned long long strtoull(const char *restrict nptr, char **restrict endptr, int base);
long long strtoll(const char *restrict nptr, char **restrict endptr, int base);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || (__STDC_VERSION__ && __STDC_VERSION__ >= 199901L) || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600)) */

void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
char *getenv(const char *name);
void abort(void);
int atexit(void (*func)(void));
int system(const char *command);

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500))
int mkstemp(char *template);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 500)) */

int mblen(const char *s, size_t n);
size_t mbstowcs(wchar_t *pwcs, const char *s, size_t n);
int mbtowc(wchar_t *pwc, const char *s, size_t n);
size_t wcstombs(char *s, const wchar_t *pwcs, size_t n);
int wctomb(char *s, wchar_t wchar);

#endif /* !STDLIB_H */
