// sigsetjmp(sigjmp_buf env, int savemask)

__asm__(".syntax unified\n\t"
        ".globl _sigsetjmp\n\t"
        ".text\n\t"
        ".align 2\n\t"
        ".arm\n\t"
        "_sigsetjmp:\n\t"
        "    str     r1, [r0, #0x70]\n\t"
        "    cmp     r1, #0\n\t"
        "    beq     __setjmp\n\t"
        "    b       _setjmp\n\t");
