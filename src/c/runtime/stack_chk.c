#include <stdint.h>
#include <unistd.h>

uintptr_t __stack_chk_guard = 0x01010101;

__attribute__((noreturn))
void __stack_chk_fail(void)
{
    _exit(127);
}
