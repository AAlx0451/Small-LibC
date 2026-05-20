// int setjmp(jmp_buf env)

__asm__(".syntax unified\n\t"
        ".globl _setjmp\n\t"
        ".text\n\t"
        ".align 2\n\t"
        ".arm\n\t"
        "_setjmp:\n\t"
        "    str     lr, [r0, #0x20]\n\t"
        "    str     r8, [r0, #0x10]\n\t"
        "    mov     r8, r0\n\t"
        "    mov     r0, #1\n\t"
        "    mov     r1, #0\n\t"
        "    add     r2, r8, #104\n\t"
        "    bl      _sigprocmask\n\t"
        "    mov     r0, r8\n\t"
        "    ldr     r8, [r0, #0x10]\n\t"
        "    ldr     lr, [r0, #0x20]\n\t"
        "    b       __setjmp\n\t");
