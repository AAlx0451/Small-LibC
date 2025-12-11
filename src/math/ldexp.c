#include <math.h>
#include <stdint.h>
#include <errno.h>

double ldexp(double x, int exp) {
    double factor;
    
    if (x == 0.0 || !isfinite(x)) return x;
    if (exp == 0) return x;

    /* Break large shifts into smaller steps to avoid overflow in factor calc */
    if (exp > 1023) {
        x *= 8.9884656743115795e307; /* 2^1023 */
        exp -= 1023;
        if (exp > 1023) { 
            errno = ERANGE; 
            return (x > 0) ? HUGE_VAL : -HUGE_VAL; 
        }
    } else if (exp < -1022) {
        x *= 2.2250738585072014e-308; /* 2^-1022 */
        exp += 1022;
        if (exp < -1022) { 
            /* Underflow to zero usually doesn't set ERANGE in strictly C89 but commonly does */
            return (x > 0) ? 0.0 : -0.0; 
        }
    }

    union { double d; uint64_t u; } u;
    u.u = ((uint64_t)(exp + 1023) << 52);
    factor = u.d;
    
    return x * factor;
}
