// stdlib.h

#ifndef STDLIB_H
#define STDLIB_H
#include <stddef.h>

typedef struct {
	int quot;
	int rem;
} div_t;

typedef struct {
	long quot;
	long rem;
} ldiv_t;

#ifndef NULL
#define NULL ((void *)0)
#endif

int abs(int j);
long labs(long j);
div_t div(int numer, int denom);
ldiv_t ldiv(long numer, long denom);
int atoi(const char *nptr);

void exit(int status);
void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
typedef int wchar_t;
#endif

#define RAND_MAX 32767
#define MB_CUR_MAX 1

long atol(const char *nptr);
int rand(void);
void srand(unsigned int seed);
int mblen(const char *s, size_t n);
int mbtowc(wchar_t *pwc, const char *s, size_t n);

int wctomb(char *s, wchar_t wchar);
size_t mbstowcs(wchar_t *pwcs, const char *s, size_t n);
size_t wcstombs(char *s, const wchar_t *pwcs, size_t n);
void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, 
		              int (*compar)(const void *, const void *));
void abort(void);

int atexit(void (*func)(void));
void exit(int status);
void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
long strtol(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);

double atof(const char *nptr);
double strtod(const char *nptr, char **endptr);
char *getenv(const char *name);
int system(const char *command);

#endif // STDLIB_H
