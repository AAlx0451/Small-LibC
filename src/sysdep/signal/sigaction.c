#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stddef.h>

/* 
 * Kernel ABI struct (16 bytes).
 * Includes explicit trampoline pointer.
 */
struct __kern_sigaction {
    union {
        void (*__sa_handler)(int);
        void (*__sa_sigaction)(int, void *, void *);
    } __sigaction_u;
    void (*sa_tramp)(void); 
    sigset_t sa_mask;
    int      sa_flags;
};

void __attribute__((naked)) __sigtramp(void) {
    __asm__ volatile (
        "push {r4, r5, lr}\n\t"   /* Save callee-saved regs and LR */
        
        "mov r5, r0\n\t"          /* Save Handler (R0) to R5 */
        "ldr r4, [sp, #12]\n\t"   /* Load ucontext from stack (offset 12 due to push) */

        /* Setup args for handler(signo, info, ctx) */
        "mov r0, r2\n\t"          /* R2 (signo) -> R0 */
        "mov r1, r3\n\t"          /* R3 (info)  -> R1 */
        "mov r2, r4\n\t"          /* ucontext   -> R2 */

        "blx r5\n\t"              /* Call user handler */

        /* Setup args for sigreturn(ctx, style) */
        "mov r0, r4\n\t"          /* ucontext */
        "mov r1, #0x1e\n\t"       /* UC_FLAVOR (30) */
        
        "mov r12, #184\n\t"       /* SYS_sigreturn */
        "svc #0x80\n\t"
        
        "nop\n\t"                 /* Crash barrier */
    );
}

int sigaction(int sig, const struct sigaction *act, struct sigaction *oact) {
    struct __kern_sigaction k_act;
    struct __kern_sigaction k_oact;
    int ret;

    if (sig <= 0 || sig >= NSIG) {
        return -1;
    }

    if (act) {
        k_act.__sigaction_u.__sa_handler = act->sa_handler;
        k_act.sa_tramp = __sigtramp;
        k_act.sa_mask = act->sa_mask;
        /* Inform kernel we provide the trampoline */
        k_act.sa_flags = act->sa_flags | SA_USERTRAMP;
    }

    ret = (int)syscall(SYS_sigaction, 
                       (long)sig, 
                       (long)(act ? &k_act : NULL), 
                       (long)(oact ? &k_oact : NULL));

    if (ret == 0 && oact) {
        oact->sa_handler = k_oact.__sigaction_u.__sa_handler;
        oact->sa_mask = k_oact.sa_mask;
        /* Hide kernel implementation detail from user */
        oact->sa_flags = k_oact.sa_flags & ~SA_USERTRAMP;
    }

    return ret;
}
