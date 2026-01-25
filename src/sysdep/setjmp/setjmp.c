// This assemly file defines
// setjmp(), _setjmp() and sigsetjmp()

__asm__(
    ".syntax unified\n\t"
    ".globl _sigsetjmp\n\t"
    ".globl _setjmp\n\t"
    ".globl __setjmp\n\t"
    ".text\n\t"
    ".align 2\n\t"
    ".arm\n\t"

    "_sigsetjmp:\n\t"
    "    str     r1, [r0, #0x70]\n\t"
    "    tst     r1, #0\n\t"
    "    beq     __setjmp\n\t"

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

    "__setjmp:\n\t"
    "    mov     ip, sp\n\t"
    "    stm     r0!, {r4, r5, r6, r7, r8, r10, r11, ip, lr}\n\t"
    "    vstmia  r0, {d8-d15}\n\t"
    "    mov     r0, #0\n\t"
    "    bx      lr\n\t"
);
