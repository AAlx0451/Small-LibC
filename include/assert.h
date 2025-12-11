#ifndef _ASSERT_H
#define _ASSERT_H

#undef assert

#ifdef NDEBUG
    #define assert(expression) ((void)0)
#else

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static inline void __assert_internal_int_to_str(unsigned int n, char *buf) {
    char *p = buf; if (n == 0) { *p++ = '0'; } else { char t[11]; int i = 0; do { t[i++] = (n % 10) + '0'; } while (n /= 10); while (i > 0) { *p++ = t[--i]; } } *p = '\0';
}

__attribute__((noreturn))
static inline void __assert_fail(const char *expression, const char *file, unsigned int line, const char *function) {
    char line_buf[11]; write(2, "Assertion failed: ", 18); write(2, expression, strlen(expression)); write(2, ", function ", 12); write(2, function, strlen(function)); write(2, ", file ", 7); write(2, file, strlen(file)); write(2, ", line ", 7); __assert_internal_int_to_str(line, line_buf); write(2, line_buf, strlen(line_buf)); write(2, ".\n", 2); abort();
}

#define assert(expression) \
    ((expression) \
        ? ((void)0) \
        : __assert_fail(#expression, __FILE__, __LINE__, __func__))

#endif // NDEBUG

#endif // _ASSERT_H
