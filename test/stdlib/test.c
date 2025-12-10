#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

static char* itoa(int n, char* s) {
    char* p = s, t;
    unsigned u = n < 0 ? -n : n;
    do { *p++ = u % 10 + '0'; } while(u /= 10);
    if (n < 0) *p++ = '-';
    *p = '\0';
    for (char* e = s; e < --p; e++) { t = *e; *e = *p; *p = t; }
    return s;
}

static void print_string(const char *s) {
    write(1, s, strlen(s));
}

static void print_number(int n) {
    char buffer[20];
    itoa(n, buffer);
    print_string(buffer);
}

static int g_failures = 0;

static void print_test_header(const char* test_name) {
    print_string("[ RUN      ] ");
    print_string(test_name);
    print_string("\n");
}

static void report_failure(const char* msg) {
    print_string("    -> Failure: ");
    print_string(msg);
    print_string("\n");
}

static void print_test_result(const char* test_name, int success) {
    if (success) {
        print_string("[       OK ] ");
    } else {
        print_string("[     FAIL ] ");
        g_failures++;
    }
    print_string(test_name);
    print_string("\n");
}

void test_abs_and_labs() {
    const char* test_name = "abs/labs";
    print_test_header(test_name);

    int success = 1;
    if (abs(-15) != 15) { success = 0; report_failure("abs(-15) != 15"); }
    if (abs(15) != 15) { success = 0; report_failure("abs(15) != 15"); }
    if (abs(0) != 0) { success = 0; report_failure("abs(0) != 0"); }
    if (labs(-123456L) != 123456L) { success = 0; report_failure("labs(-123456L) != 123456L"); }
    if (labs(123456L) != 123456L) { success = 0; report_failure("labs(123456L) != 123456L"); }
    if (labs(0L) != 0L) { success = 0; report_failure("labs(0L) != 0L"); }

    print_test_result(test_name, success);
}

void test_div_and_ldiv() {
    const char* test_name = "div/ldiv";
    print_test_header(test_name);

    int success = 1;
    div_t d_res = div(10, 3);
    if (d_res.quot != 3 || d_res.rem != 1) { success = 0; report_failure("div(10, 3)"); }

    d_res = div(-10, 3);
    if (d_res.quot != -3 || d_res.rem != -1) { success = 0; report_failure("div(-10, 3)"); }

    ldiv_t ld_res = ldiv(100000L, 300L);
    if (ld_res.quot != 333L || ld_res.rem != 100L) { success = 0; report_failure("ldiv(100000L, 300L)"); }

    print_test_result(test_name, success);
}

void test_string_conversions() {
    const char* test_name = "atoi/atol/atof/strtod";
    print_test_header(test_name);

    int success = 1;
    if (atoi("123") != 123) { success = 0; report_failure("atoi(\"123\")"); }
    if (atoi("  -45") != -45) { success = 0; report_failure("atoi(\"  -45\")"); }
    if (atoi("0") != 0) { success = 0; report_failure("atoi(\"0\")"); }
    if (atoi("12a34") != 12) { success = 0; report_failure("atoi(\"12a34\")"); }

    if (atol("987654321") != 987654321L) { success = 0; report_failure("atol(\"987654321\")"); }
    if (atol("-100000") != -100000L) { success = 0; report_failure("atol(\"-100000\")"); }
    
    double val_atof = atof("3.14");
    if (val_atof < 3.13 || val_atof > 3.15) { success = 0; report_failure("atof(\"3.14\")"); }
    
    val_atof = atof("-0.001");
    if (val_atof < -0.0011 || val_atof > -0.0009) { success = 0; report_failure("atof(\"-0.001\")"); }

    char *endptr;
    double val_strtod = strtod("  -123.45e-1 junk", &endptr);
    if (val_strtod < -12.346 || val_strtod > -12.344) { success = 0; report_failure("strtod value incorrect"); }
    if (strncmp(endptr, " junk", 5) != 0) { success = 0; report_failure("strtod endptr incorrect"); }
    
    const char* start = "not_a_number";
    val_strtod = strtod(start, &endptr);
    if (val_strtod != 0.0 || endptr != start) { success = 0; report_failure("strtod on invalid string"); }

    print_test_result(test_name, success);
}

void test_rand_srand() {
    const char* test_name = "rand/srand";
    print_test_header(test_name);

    int success = 1;
    int seq1[5], seq2[5], seq3[5];

    srand(42);
    for(int i = 0; i < 5; ++i) seq1[i] = rand();
    
    srand(42);
    for(int i = 0; i < 5; ++i) seq2[i] = rand();
    
    srand(99);
    for(int i = 0; i < 5; ++i) seq3[i] = rand();

    if (memcmp(seq1, seq2, sizeof(seq1)) != 0) {
        success = 0;
        report_failure("rand() sequence not repeatable with same seed.");
    }
    
    if (memcmp(seq1, seq3, sizeof(seq1)) == 0) {
        success = 0;
        report_failure("rand() sequence was the same for different seeds.");
    }

    print_test_result(test_name, success);
}

static int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

void test_qsort() {
    const char* test_name = "qsort";
    print_test_header(test_name);

    int success = 1;
    int numbers[] = { 40, 10, 100, 90, 20, 25, 10, 5 };
    size_t count = sizeof(numbers) / sizeof(numbers[0]);

    qsort(numbers, count, sizeof(int), compare_ints);

    for (size_t i = 0; i < count - 1; ++i) {
        if (numbers[i] > numbers[i+1]) {
            success = 0;
            report_failure("array not sorted correctly.");
            break;
        }
    }
    
    int expected[] = { 5, 10, 10, 20, 25, 40, 90, 100 };
    if (success && memcmp(numbers, expected, sizeof(numbers)) != 0) {
        success = 0;
        report_failure("sorted array does not match expected result.");
    }

    print_test_result(test_name, success);
}

void test_getenv() {
    const char* test_name = "getenv";
    print_test_header(test_name);
    
    int success = 1;
    
    char* path = getenv("PATH");
    if (path == NULL || strlen(path) == 0) {
        success = 0;
        report_failure("getenv(\"PATH\") returned NULL or empty string.");
    }
    
    if (getenv("THIS_VARIABLE_SHOULD_NOT_EXIST_ABC123") != NULL) {
        success = 0;
        report_failure("getenv() found a non-existent variable.");
    }

    print_test_result(test_name, success);
}

void test_malloc_free() {
    const char* test_name = "malloc/free";
    print_test_header(test_name);

    int success = 1;
    char *mem = (char*)malloc(50);

    if (mem == NULL) {
        success = 0;
        report_failure("malloc returned NULL for a valid size.");
    } else {
        strcpy(mem, "Hello from malloc!");
        if (strcmp(mem, "Hello from malloc!") != 0) {
            success = 0;
            report_failure("data corruption in malloc'd memory.");
        }
        free(mem);
    }

    print_test_result(test_name, success);
}

void test_calloc() {
    const char* test_name = "calloc";
    print_test_header(test_name);

    int success = 1;
    size_t num_elements = 20;
    int *mem = (int*)calloc(num_elements, sizeof(int));

    if (mem == NULL) {
        success = 0;
        report_failure("calloc returned NULL for a valid size.");
    } else {
        for (size_t i = 0; i < num_elements; ++i) {
            if (mem[i] != 0) {
                success = 0;
                report_failure("memory from calloc was not zero-initialized.");
                break;
            }
        }
        free(mem);
    }

    print_test_result(test_name, success);
}

void test_realloc() {
    const char* test_name = "realloc";
    print_test_header(test_name);

    int success = 1;
    char *mem = (char*)realloc(NULL, 20);
    if (mem == NULL) {
        success = 0;
        report_failure("realloc(NULL, size) failed.");
    } else {
        strcpy(mem, "initial data");
    }
    
    if (success) {
        char *grown_mem = (char*)realloc(mem, 50);
        if (grown_mem == NULL) {
            success = 0;
            report_failure("realloc to a larger size failed.");
        } else {
            mem = grown_mem;
            if (strcmp(mem, "initial data") != 0) {
                success = 0;
                report_failure("data lost after realloc (grow).");
            }
        }
    }

    if (success) {
        char *shrunk_mem = (char*)realloc(mem, 8);
        if (shrunk_mem == NULL) {
            success = 0;
            report_failure("realloc to a smaller size failed.");
        } else {
            mem = shrunk_mem;
            if (strncmp(mem, "initial", 7) != 0) {
                success = 0;
                report_failure("data lost after realloc (shrink).");
            }
        }
    }

    if (mem != NULL) {
        realloc(mem, 0);
    }

    print_test_result(test_name, success);
}

void test_exit() {
    const char* test_name = "exit";
    print_test_header(test_name);

    int success = 1;
    const int exit_code = 55;

    pid_t pid = fork();

    if (pid < 0) {
        success = 0;
        report_failure("fork() failed for exit test.");
    } else if (pid == 0) {
        exit(exit_code);
    } else {
        int status;
        wait(&status);
        if (!WIFEXITED(status)) {
            success = 0;
            report_failure("child process did not terminate normally via exit().");
        } else if (WEXITSTATUS(status) != exit_code) {
            success = 0;
            report_failure("child process exited with wrong status via exit().");
        }
    }

    print_test_result(test_name, success);
}


int main() {
    print_string("========== StdLib Test Suite ==========\n");

    test_abs_and_labs();
    test_div_and_ldiv();
    test_string_conversions();
    test_rand_srand();
    test_qsort();
    test_malloc_free();
    test_calloc();
    test_realloc();
    test_getenv();
    test_exit();

    print_string("=======================================\n");
    if (g_failures == 0) {
        print_string("All tests passed.\n");
    } else {
        print_string("Tests finished with ");
        print_number(g_failures);
        print_string(" failure(s).\n");
    }

    return g_failures;
}
