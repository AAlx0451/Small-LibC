// void siglongjmp(sigjmp_buf env, int val)

__asm__(".syntax unified\n\t"
        ".globl _siglongjmp\n\t"
        ".text\n\t"
        ".align 2\n\t"
        ".arm\n\t"
        "_siglongjmp:\n\t"
        "    ldr     r2, [r0, #0x70]\n\t"
        "    cmp     r2, #0\n\t"
        "    beq     __longjmp\n\t"
        "    b       _longjmp\n\t");
