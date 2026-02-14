#ifndef SETJMP_H
#define SETJMP_H

#include <features.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wbuiltin-requires-header"
#endif

typedef int jmp_buf[32]; 

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

#if defined(_DARWIN_C_SOURCE)
int _setjmp(jmp_buf env);
void _longjmp(jmp_buf env, int val);
#endif /* _DARWIN_C_SOURCE */

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
typedef int sigjmp_buf[32];
int sigsetjmp(sigjmp_buf env, int savemask);
void siglongjmp(sigjmp_buf env, int val);
#endif /* !_ANSI && (_DARWIN_C_SOURCE || _POSIX_C_SOURCE || _XOPEN_SOURCE) */

#endif /* SETJMP_H */
