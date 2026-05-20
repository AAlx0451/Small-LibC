// void _longjmp(jmp_buf env, int val)

__asm__(".syntax unified\n\t"
        ".globl __longjmp\n\t"
        ".text\n\t"
        ".align 2\n\t"
        ".arm\n\t"
        "__longjmp:\n\t"
        "    ldm     r0!, {r4, r5, r6, r7, r8, r10, r11, ip, lr}\n\t"
        "    mov     sp, ip\n\t"
        "    vldmia  r0, {d8-d15}\n\t"
        "    movs    r0, r1\n\t"
        "    moveq   r0, #1\n\t"
        "    bx      lr\n\t");
