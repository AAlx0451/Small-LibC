#include <stdio.h>
#include <time.h>
#include <unistd.h>  

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { \
            printf(GREEN("[PASS]") " %s\n", msg); \
            tests_passed++; \
        } else { \
            printf(RED("[FAIL]") " %s\n", msg); \
            tests_failed++; \
        } \
    } while(0)

static void burn_cpu(int iterations) {
    volatile long x = 0;
    for (int i = 0; i < iterations; i++) {
        x += i * i;
    }
    (void)x;
}

void clock_test() {  
    clock_t t1, t2;
    double diff_sec;

    // Basic validity check
    t1 = clock();
    TEST_ASSERT(t1 != (clock_t)-1, "clock() returns a valid value (not -1)");

    // Monotonicity without CPU work
    t2 = clock();
    TEST_ASSERT(t2 >= t1, "clock() is monotonically non-decreasing");

    // CPU time accumulation after busy work
    burn_cpu(100000000);
    t2 = clock();
    TEST_ASSERT(t2 != (clock_t)-1, "clock() still valid after CPU burn");
    diff_sec = (double)(t2 - t1) / CLOCKS_PER_SEC;
    TEST_ASSERT(diff_sec > 0.01, "clock() detects CPU time (>0.01s after burn_cpu)");

    // Sleep() should no significantly increase clock() 
    t1 = clock();
    sleep(1);
    t2 = clock();
    TEST_ASSERT(t2 != (clock_t)-1, "clock() still valid after sleep");
    diff_sec = (double)(t2 - t1) / CLOCKS_PER_SEC;
    TEST_ASSERT(diff_sec < 0.05, "clock() ignores sleep() (delta < 0.05s)");

    // Overhead of repeated calls
    t1 = clock();
    for (int i = 0; i < 1000; i++) {
        (void)clock();
    }
    t2 = clock();
    diff_sec = (double)(t2 - t1) / CLOCKS_PER_SEC;
    TEST_ASSERT(diff_sec < 0.01, "clock() overhead is small (<0.01s for 1000 calls)");

    // CLOCKS_PER_SEC consistency
    TEST_ASSERT(CLOCKS_PER_SEC == 1000000, "CLOCKS_PER_SEC == 1000000 as defined in time.h");

    printf("\n=== clock tests: %d passed, %d failed ===\n",
           tests_passed, tests_failed);
}
