#ifndef STDLIB_H
#define STDLIB_H

# include <stddef.h>

/* macros */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

/* types */
typedef struct {
  int quot;
  int rem;
} div_t;

typedef struct {
  long quot;
  long rem;
} ldiv_t;

/* functions */
void free(void *ptr);
void *malloc(size_t size);
void *calloc(size_t number, size_t size);
void *realloc(void *p, size_t size);

__attribute__((noreturn)) void exit(int status);

int abs(int num);
long labs(long num);
div_t div(int numer, int denom);
ldiv_t ldiv(long numer, long denom);

#endif
