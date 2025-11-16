#include <stdarg.h>
#include <errno.h>

int errno;

long syscall(long number, ...) {
  va_list args;
  long arg1, arg2, arg3, arg4, arg5, arg6;
  long result;
  
  va_start(args, number);
  arg1 = va_arg(args, long);
  arg2 = va_arg(args, long);
  arg3 = va_arg(args, long);
  arg4 = va_arg(args, long);
  arg5 = va_arg(args, long);
  arg6 = va_arg(args, long);
  va_end(args);

  register long r0 __asm__("r0") = arg1;
  register long r1 __asm__("r1") = arg2;
  register long r2 __asm__("r2") = arg3;
  register long r3 __asm__("r3") = arg4;
  register long r12 __asm__("r12") = number;
  
  __asm__ volatile (
    "sub sp, sp, #8\n\t"
    "str %[arg5], [sp]\n\t"
    "str %[arg6], [sp, #4]\n\t"
    "svc #0x80\n\t"
    
    "add sp, sp, #8\n\t"
    "bcs 1f\n\t"
    "b 2f\n\t"
    "1:\n\t"
    
    "2:"
    : "=r" (result) 
    : [arg5]"r"(arg5), [arg6]"r"(arg6),
      "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r12)
    : "memory", "cc"
  );
  
  long error_flag;
  __asm__ volatile (
      "mov %0, #0\n\t" // error_flag = 0
      "movcs %0, #1\n\t" // if (carry set) error_flag = 1
      : "=r"(error_flag)
      :
      : "cc"
  );
  
  if (error_flag) {
    errno = result;
    return -1;
  }
  
  return result;
}
