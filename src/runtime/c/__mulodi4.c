#include "runtime.h"
#include <limits.h>

long long __mulodi4(long long a, long long b, int *overflow)
{
    long long result;
    *overflow = 0;

    if (a == 0 || b == 0)
        return 0;

    /* avoid -2^63/-1 = -2^63 */
    if (a == LLONG_MIN) {
        if (b != 1) {
            *overflow = 1;
        }
        return (long long)((unsigned long long)a * (unsigned long long)b);
    }
    if (b == LLONG_MIN) {
        if (a != 1) {
            *overflow = 1;
        }
        return (long long)((unsigned long long)a * (unsigned long long)b);
    }

    /* multiply */
    result = (long long)((unsigned long long)a * (unsigned long long)b);

    /* check if overflow */
    if (result / a != b) {
        *overflow = 1;
    }

    return result;
}
