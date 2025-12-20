#ifndef LIBC_H
#define LIBC_H

/*
 * This header is a general
 * "entry point" to
 * ANSI C
 */
 
// Small-LibC's
#include <assert.h> // full
#include <ctype.h> // full but ascii only
#include <errno.h> // ansi full but not posix
#include <math.h> // ieee 754 and ascii full
#include <setjmp.h> // full
#include <signal.h> // no signal() rignt now
#include <stdio.h> // full
#include <stdlib.h> // almost full, no wide characters
#include <string.h> // full but no locales support
#include <time.h> // full
/* #include <locale.h */ // not implemented yet

// Apple's included in Small-LibC
#include <stdint.h>

// Compiler-dependent
#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>

#endif
