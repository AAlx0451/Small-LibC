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
    if (abs(-15) != 15) success = 0;
    if (abs(15) != 15) success = 0;
    if (abs(0) != 0) success = 0;
    if (labs(-123456L) != 123456L) success = 0;
    if (labs(123456L) != 123456L) success = 0;
    if (labs(0L) != 0L) success = 0;

    print_test_result(test_name, success);
}

void test_div_and_ldiv() {
    const char* test_name = "div/ldiv";
    print_test_header(test_name);

    int success = 1;
    div_t d_res = div(10, 3);
    if (d_res.quot != 3 || d_res.rem != 1) success = 0;
    
    d_res = div(-10, 3);
    if (d_res.quot != -3 || d_res.rem != -1) success = 0;

    ldiv_t ld_res = ldiv(100000L, 300L);
    if (ld_res.quot != 333L || ld_res.rem != 100L) success = 0;

    print_test_result(test_name, success);
}

void test_malloc_free() {
    const char* test_name = "malloc/free";
    print_test_header(test_name);

    int success = 1;
    char *mem = (char*)malloc(50);

    if (mem == NULL) {
        success = 0;
        print_string("Error: malloc returned NULL for a valid size.\n");
    } else {
        strcpy(mem, "Hello from malloc!");
        if (strcmp(mem, "Hello from malloc!") != 0) {
            success = 0;
            print_string("Error: data corruption in malloc'd memory.\n");
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
        print_string("Error: calloc returned NULL for a valid size.\n");
    } else {
        for (size_t i = 0; i < num_elements; ++i) {
            if (mem[i] != 0) {
                success = 0;
                print_string("Error: memory from calloc was not zero-initialized.\n");
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
        print_string("Error: realloc(NULL, size) failed.\n");
    } else {
        strcpy(mem, "initial data");
    }
    if (success) {
        char *grown_mem = (char*)realloc(mem, 50);
        if (grown_mem == NULL) {
            success = 0;
            print_string("Error: realloc to a larger size failed.\n");
        } else {
            mem = grown_mem;
            if (strcmp(mem, "initial data") != 0) {
                success = 0;
                print_string("Error: data lost after realloc (grow).\n");
            }
        }
    }

    if (success) {
        char *shrunk_mem = (char*)realloc(mem, 8);
        if (shrunk_mem == NULL) {
            success = 0;
            print_string("Error: realloc to a smaller size failed.\n");
        } else {
            mem = shrunk_mem;
            if (strncmp(mem, "initial", 7) != 0) {
                success = 0;
                print_string("Error: data lost after realloc (shrink).\n");
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
        print_string("Error: fork() failed for exit test.\n");
    } else if (pid == 0) {
        exit(exit_code);
    } else {
        int status;
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != exit_code) {
            success = 0;
            print_string("Error: child process exited with wrong status via exit().\n");
        }
    }

    print_test_result(test_name, success);
}


int main() {
    print_string("========== StdLib Test Suite ==========\n");

    test_abs_and_labs();
    test_div_and_ldiv();
    test_malloc_free();
    test_calloc();
    test_realloc();
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
