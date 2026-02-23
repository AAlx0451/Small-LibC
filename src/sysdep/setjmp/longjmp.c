// this assembly file defines
// longjmp(), _longjmp() and siglongjmp()

__asm__(
    ".syntax unified\n\t"
    ".globl _siglongjmp\n\t"
    ".globl _longjmp\n\t"
    ".globl __longjmp\n\t"
    ".text\n\t"
    ".align 2\n\t"
    ".arm\n\t"

    "_siglongjmp:\n\t"
    "    ldr     r2, [r0, #0x70]\n\t"
    "    tst     r2, #0\n\t"
    "    beq     __longjmp\n\t"

    "_longjmp:\n\t"
    "    mov     r6, r0\n\t"
    "    mov     r8, r1\n\t"
    "    mov     r0, #3\n\t"
    "    add     r1, r6, #104\n\t"
    "    mov     r2, #0\n\t"
    "    bl      _sigprocmask\n\t"
    "    mov     r1, r8\n\t"
    "    mov     r0, r6\n\t"

    "__longjmp:\n\t"
    "    ldm     r0!, {r4, r5, r6, r7, r8, r10, r11, ip, lr}\n\t"
    "    mov     sp, ip\n\t"
    "    vldmia  r0, {d8-d15}\n\t"
    "    movs    r0, r1\n\t"
    "    moveq   r0, #1\n\t"
    "    bx      lr\n\t");
