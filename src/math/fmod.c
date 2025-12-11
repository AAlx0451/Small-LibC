#include <math.h>
#include <stdint.h>
#include <errno.h>

double fmod(double x, double y) {
    union { double f; uint64_t i; } ux, uy;
    ux.f = x;
    uy.f = y;
    int exp_x = (ux.i >> 52) & 0x7FF;
    int exp_y = (uy.i >> 52) & 0x7FF;
    if (exp_x == 0x7FF || y == 0.0 || (exp_y == 0x7FF && (uy.i & 0xFFFFFFFFFFFFFULL) != 0)) {
        return NAN;
    }

    if (exp_y == 0x7FF) {
        return x;
    }

    ux.i &= 0x7FFFFFFFFFFFFFULL; // abs(x)
    uy.i &= 0x7FFFFFFFFFFFFFULL; // abs(y)
    if (ux.f < uy.f) {
        return x;
    }

    if (ux.f == uy.f) {
        return 0.0 * x; 
    }
    
    double abs_x = ux.f;
    double abs_y = uy.f;
    while (abs_x >= 2.0 * abs_y && abs_x < HUGE_VAL) {
        abs_y *= 2.0;
    }
    
    while (abs_x >= uy.f) {
        if (abs_x >= abs_y) {
            abs_x -= abs_y;
        }
        abs_y *= 0.5;
    }

    if (x < 0) {
        return -abs_x;
    }
    return abs_x;
}
