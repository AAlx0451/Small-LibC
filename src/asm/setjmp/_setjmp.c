// int _setjmp(jmp_buf env)

__asm__(".syntax unified\n\t"
        ".globl __setjmp\n\t"
        ".text\n\t"
        ".align 2\n\t"
        ".arm\n\t"
        "__setjmp:\n\t"
        "    mov     ip, sp\n\t"
        "    stm     r0!, {r4, r5, r6, r7, r8, r10, r11, ip, lr}\n\t"
        "    vstmia  r0, {d8-d15}\n\t"
        "    mov     r0, #0\n\t"
        "    bx      lr\n\t");
