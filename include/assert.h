#ifndef ASSERT_H
#define ASSERT_H

#undef assert

#ifdef NDEBUG
    #define assert(expr) ((void)0)
#else

#include <unistd.h>
#include <stdlib.h>

#if defined(__GNUC) || defined(__clang__)
# define always __attribute__((always_inline))
# define noreturn __attribute__((noreturn))
# define cold __attribute__((cold))
#else
# define always
# define noreturn
# define cold
#endif

void static always inline write_str(const char* s) {
    if(s) {
        size_t len = 0;
        while (s[len]) len++;
        write(STDERR_FILENO, s, len);
    }
}

static noreturn cold void __assert_fail_internal(const char *expr, const char *file, int line, const char *func) {
    char buf[12];
    char *p = buf + sizeof(buf) - 1;
    *p = '\0';
    if (line == 0) *--p = '0';
    else {
        for (; line > 0; line /= 10) *--p = (char)('0' + (line % 10));
    }

    write_str("Assertion failed: ");
    write_str(expr);
    write_str(" (file: ");
    write_str(file);
    write_str(", line: ");
    write_str(p);
    write_str(", function: ");
    write_str(func);
    write_str(")\n");
    
    abort();
}

#define assert(expr) \
    ((void)((expr) ? (void)0 : __assert_fail_internal(#expr, __FILE__, __LINE__, __func__)))

#endif // NDEBUG
 
#endif // ASSERT_H
