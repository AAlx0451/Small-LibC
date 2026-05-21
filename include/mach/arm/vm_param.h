#ifndef MACH_ARM_VM_PARAM_H
#define MACH_ARM_VM_PARAM_H

#include <features.h>

#if defined(_MACH_SOURCE)

#if defined(__arm__)

#define BYTE_SIZE 8 /* byte size (bits) */

#define ARM_PGBYTES 4096 /* arm pagesize in bytes */
#define ARM_PGSHIFT 12 /* page size as a power of 2 */

#define PAGE_SIZE ARM_PGBYTES
#define PAGE_SHIFT ARM_PGSHIFT
#define PAGE_MASK (PAGE_SIZE-1)

#define VM_PAGE_SIZE ARM_PGBYTES

#define machine_ptob(x) ((x) << ARM_PGSHIFT) /* pages -> bytes */

#define KERNEL_STACK_SIZE (4*ARM_PGBYTES) /* kernel stack size */
#define INTSTACK_SIZE (4*ARM_PGBYTES) /* interrupt stack size */

#define SWI_SYSCALL 0x80 /* swi #0x80 */

#if defined(_C_SOURCE)

#define VM_MIN_ADDRESS ((vm_address_t)0x00000000)
#define VM_MAX_ADDRESS ((vm_address_t)0x80000000)

#define HIGH_EXC_VECTORS ((vm_address_t)0xFFFF0000) /* arm high exception vectors base addr */

#define VM_MIN_KERNEL_ADDRESS ((vm_address_t) 0x80000000)
#define VM_MIN_KERNEL_AND_KEXT_ADDRESS VM_MIN_KERNEL_ADDRESS 
#define VM_HIGH_KERNEL_WINDOW ((vm_address_t)0xFFFE0000)
#define VM_MAX_KERNEL_ADDRESS ((vm_address_t)0xFFFEFFFF)

#define VM_KERNEL_ADDRESS(va) ((((vm_address_t)(va)) >= VM_MIN_KERNEL_ADDRESS) && \
    (((vm_address_t)(va)) <= VM_MAX_KERNEL_ADDRESS)) /* check if addr kernel */

#define MACH_VM_MIN_ADDRESS ((mach_vm_offset_t)0)
#define MACH_VM_MAX_ADDRESS ((mach_vm_offset_t)VM_MAX_ADDRESS)

#endif /* _C_SOURCE */

#endif /* __arm__ */

#endif /* _MACH_SOURCE */

#endif /* !MACH_ARM_KERN_RETURN_H */
