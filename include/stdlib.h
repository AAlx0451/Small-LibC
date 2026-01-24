#ifndef STDLIB_H
#define STDLIB_H

# include <stddef.h>
# include <unistd.h> // NORETURN

/* CONSTANTS */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define RAND_MAX 0x7FFFFFFF
# define MB_CUR_MAX 4

/* TYPES */
typedef struct {
  int quot;
  int rem;
} div_t;

typedef struct {
  long quot;
  long rem;
} ldiv_t;

typedef struct {
  long long quot;
  long long rem;
} lldiv_t;

/* FUNCTIONS */

// memory allocation
void free(void *ptr);
void *malloc(size_t size);
void *calloc(size_t number, size_t size);
void *realloc(void *p, size_t size);

// exit
NORETURN void exit(int status);
NORETURN void _Exit(int status);

// math
int abs(int num);
long labs(long num);
div_t div(int numer, int denom);
ldiv_t ldiv(long numer, long denom);
void srand(unsigned int seed);
int rand(void);
long long llabs(long long j);
lldiv_t lldiv(long long numer, long long denom);

// general
int atoi(const char *nptr);
long atol(const char *nptr);
long long atoll(const char *nptr);
double atof(const char *str);
double strtod(const char *str, char **end);
long double strtold(const char *restrict nptr, char **restrict endptr);
float strtof(const char *restrict nptr, char **restrict endptr);
unsigned long long strtoull(const char *restrict nptr, char **restrict endptr, int base);
long strtol(const char *restrict nptr, char **restrict endptr, int base);
unsigned long strtoul(const char *restrict nptr, char **restrict endptr, int base);
long long strtoll(const char *restrict nptr, char **restrict endptr, int base);
void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
char *getenv(const char *name);	
void abort(void);
int atexit(void (*func)(void));
int system(const char *command);

// posix
int mkstemp(char *template);

// utf8
int mblen(const char *s, size_t n);
size_t mbstowcs(wchar_t *pwcs, const char *s, size_t n);
int mbtowc(wchar_t *pwc, const char *s, size_t n);
size_t wcstombs(char *s, const wchar_t *pwcs, size_t n);
int wctomb(char *s, wchar_t wchar);

#endif
