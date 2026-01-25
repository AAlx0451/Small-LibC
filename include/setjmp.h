#ifndef SETJMP_H
#define SETJMP_H

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-requires-header"
#endif

typedef int jmp_buf[32]; 
typedef int sigjmp_buf[32];

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

int _setjmp(jmp_buf env);
void _longjmp(jmp_buf env, int val);

int sigsetjmp(sigjmp_buf env, int savemask);
void siglongjmp(sigjmp_buf env, int val);

#endif /* SETJMP_H */
