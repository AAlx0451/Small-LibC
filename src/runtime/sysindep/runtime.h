#include <stdint.h>
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

double __floatdidf(long long a);
double __floatundidf(unsigned long long a);
int __divsi3(int n, int d);
int __modsi3(int n, int d);
long long __divdi3(long long n, long long d);
long long __fixdfdi(double a);
long long __moddi3(long long n, long long d);
unsigned int __udivsi3(unsigned int n, unsigned int d);
unsigned int __umodsi3(unsigned int n, unsigned int d);
unsigned long long __fixunsdfdi(double a);
unsigned long long __udivdi3(unsigned long long n, unsigned long long d);
unsigned long long __umoddi3(unsigned long long n, unsigned long long d);
void __stack_chk_fail(void);
void __stack_chk_guard_init(void);
extern uintptr_t __stack_chk_guard;
