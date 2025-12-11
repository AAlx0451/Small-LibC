#ifndef STDLIB_H
#define STDLIB_H

# include <stddef.h>
# include <unistd.h> // NORETURN

/* CONSTANTS */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define RAND_MAX 0x7FFFFFFF

/* TYPES */
typedef struct {
  int quot;
  int rem;
} div_t;

typedef struct {
  long quot;
  long rem;
} ldiv_t;

/* FUNCTIONS */

// memory allocation
void free(void *ptr);
void *malloc(size_t size);
void *calloc(size_t number, size_t size);
void *realloc(void *p, size_t size);

// exit
NORETURN void exit(int status);

// math
int abs(int num);
long labs(long num);
div_t div(int numer, int denom);
ldiv_t ldiv(long numer, long denom);
void srand(uint64_t seed);
uint32_t rand(void);

// general
int atoi(const char *nptr);
long atol(const char *nptr);
double atof(const char *str);
double strtod(const char *str, char **end);
void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
char *getenv(const char *name);	
void abort(void);
int atexit(void (*func)(void));
void _call_atexit_handlers(void);

#endif
