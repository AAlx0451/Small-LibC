#ifndef WCHAR_H
#define WCHAR_H

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdint.h>

#ifndef WCHAR_MIN
#define WCHAR_MIN (-2147483647 - 1)
#endif

#ifndef WCHAR_MAX
#define WCHAR_MAX (2147483647)
#endif

#ifndef WEOF
#define WEOF ((wint_t)(-1))
#endif

typedef int wint_t;

typedef struct
{
    int __count;
    union
    {
        unsigned int __wch;
        char __wchb[8];
    } __value;
} mbstate_t;

#endif
