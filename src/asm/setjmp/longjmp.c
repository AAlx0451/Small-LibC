// void longjmp(jmp_buf env, int val)

__asm__(".syntax unified\n\t"
        ".globl _longjmp\n\t"
        ".text\n\t"
        ".align 2\n\t"
        ".arm\n\t"
        "_longjmp:\n\t"
        "    mov     r6, r0\n\t"
        "    mov     r8, r1\n\t"
        "    mov     r0, #3\n\t"
        "    add     r1, r6, #104\n\t"
        "    mov     r2, #0\n\t"
        "    bl      _sigprocmask\n\t"
        "    mov     r1, r8\n\t"
        "    mov     r0, r6\n\t"
        "    b       __longjmp\n\t");
