#include "runtime.h"
#include <stdlib.h>
#include <unistd.h>

void __noreturn __chk_fail(void)
{
    const char msg[] = "*** buffer overflow detected ***: terminated\n";
    write(2, msg, sizeof(msg) - 1);
    abort();
}
