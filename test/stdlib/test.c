#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h> /* Для snprintf, раз уж он реализован в libc */

/* --- Helpers --- */

static int g_failures = 0;

static void print_string(const char *s) {
    write(1, s, strlen(s));
}

static void print_number(long long n) {
    char buffer[32];
    /* Используем простую реализацию, чтобы не зависеть от sprintf в базовых хелперах,
       хотя sprintf у вас есть. */
    char* p = buffer;
    long long temp = n;
    unsigned long long u = n < 0 ? -n : n;
    
    do { 
        *p++ = (u % 10) + '0'; 
    } while(u /= 10);
    
    if (n < 0) *p++ = '-';
    *p = '\0';
    
    // Reverse
    char t;
    for (char* e = buffer; e < --p; e++) { 
        t = *e; *e = *p; *p = t; 
    }
    print_string(buffer);
}

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

/* --- Tests --- */

void test_abs_and_labs() {
    const char* test_name = "abs/labs/llabs";
    print_test_header(test_name);

    int success = 1;
    if (abs(-15) != 15) { success = 0; report_failure("abs(-15)"); }
    if (labs(-123456L) != 123456L) { success = 0; report_failure("labs(-123456L)"); }
    if (llabs(-9223372036854775807LL) != 9223372036854775807LL) { 
        success = 0; report_failure("llabs(LLONG_MAX)"); 
    }

    print_test_result(test_name, success);
}

void test_div_ldiv_lldiv() {
    const char* test_name = "div/ldiv/lldiv";
    print_test_header(test_name);

    int success = 1;
    
    div_t d = div(-10, 3);
    if (d.quot != -3 || d.rem != -1) { success = 0; report_failure("div"); }

    ldiv_t ld = ldiv(100000L, 300L);
    if (ld.quot != 333L || ld.rem != 100L) { success = 0; report_failure("ldiv"); }

    lldiv_t lld = lldiv(1234567890123LL, 100LL);
    if (lld.quot != 12345678901LL || lld.rem != 23LL) { success = 0; report_failure("lldiv"); }

    print_test_result(test_name, success);
}

void test_basic_conversions() {
    const char* test_name = "atoi/atol/atoll/atof";
    print_test_header(test_name);

    int success = 1;
    if (atoi(" -123") != -123) { success = 0; report_failure("atoi"); }
    if (atol(" 987654") != 987654L) { success = 0; report_failure("atol"); }
    if (atoll(" -1234567890123") != -1234567890123LL) { success = 0; report_failure("atoll"); }
    
    double v = atof("3.14159");
    if (v < 3.1415 || v > 3.1416) { success = 0; report_failure("atof"); }

    print_test_result(test_name, success);
}

void test_advanced_str_to_int() {
    const char* test_name = "strtol/strtoul/strtoll/strtoull";
    print_test_header(test_name);
    int success = 1;
    char *end;

    // Base 10
    long l = strtol(" -123junk", &end, 10);
    if (l != -123 || strcmp(end, "junk") != 0) { success = 0; report_failure("strtol base 10"); }

    // Base 16
    unsigned long ul = strtoul("0xFF", &end, 16);
    if (ul != 255) { success = 0; report_failure("strtoul base 16"); }

    // Base 0 (detect hex)
    long long ll = strtoll("0x100000000", &end, 0);
    if (ll != 4294967296LL) { success = 0; report_failure("strtoll base 0"); }

    // Base 36
    unsigned long long ull = strtoull("Z", &end, 36);
    if (ull != 35) { success = 0; report_failure("strtoull base 36"); }

    // Errno check (Overflow) - предполагаем, что sizeof(long) <= 8
    errno = 0;
    // Очень большое число для проверки переполнения
    strtoul("9999999999999999999999999999999", NULL, 10);
    if (errno != ERANGE) { success = 0; report_failure("strtoul errno ERANGE"); }

    print_test_result(test_name, success);
}

void test_advanced_str_to_float() {
    const char* test_name = "strtod/strtof/strtold";
    print_test_header(test_name);
    int success = 1;
    char *end;

    double d = strtod("1.5e2", &end);
    if (d != 150.0) { success = 0; report_failure("strtod sci notation"); }

    float f = strtof("0.125", NULL);
    if (f != 0.125f) { success = 0; report_failure("strtof"); }

    long double ld = strtold("10.0", NULL);
    if (ld != 10.0L) { success = 0; report_failure("strtold"); }

    print_test_result(test_name, success);
}

static int int_cmp(const void *a, const void *b) {
    return (*(const int *)a - *(const int *)b);
}

void test_searching_sorting() {
    const char* test_name = "qsort/bsearch";
    print_test_header(test_name);
    int success = 1;

    int values[] = { 50, 20, 60, 10, 30 };
    qsort(values, 5, sizeof(int), int_cmp);

    if (values[0] != 10 || values[4] != 60) {
        success = 0; report_failure("qsort bad order");
    }

    // Тестируем bsearch на отсортированном массиве: 10, 20, 30, 50, 60
    int key = 30;
    int *found = (int*)bsearch(&key, values, 5, sizeof(int), int_cmp);
    if (!found || *found != 30) {
        success = 0; report_failure("bsearch failed to find existing key");
    }

    key = 40; // нет в массиве
    found = (int*)bsearch(&key, values, 5, sizeof(int), int_cmp);
    if (found != NULL) {
        success = 0; report_failure("bsearch found non-existent key");
    }

    print_test_result(test_name, success);
}

void test_env_system() {
    const char* test_name = "getenv/system";
    print_test_header(test_name);
    int success = 1;

    // getenv
    if (getenv("PATH") == NULL) { success = 0; report_failure("getenv(PATH) is NULL"); }
    
    // system
    // system(NULL) проверяет наличие командного процессора
    if (system(NULL) == 0) { 
        // Это не обязательно ошибка (мб нет shell), но предупредим
        print_string("    -> Info: system(NULL) returned 0 (no shell?)\n");
    } else {
        // Если shell есть, попробуем простую команду
        int ret = system("true"); // или exit 0
        if (ret != 0) {
             // Некоторые реализации system возвращают статус waitpid, 
             // другие - код возврата. Проверим WEXITSTATUS если нужно.
             if (WEXITSTATUS(ret) != 0) {
                success = 0; report_failure("system(\"true\") returned non-zero");
             }
        }
    }

    print_test_result(test_name, success);
}

/* Обработчик для atexit */
static void atexit_handler(void) {
    // Пишем 'X' в stdout
    char c = 'X';
    write(1, &c, 1);
}

void test_atexit() {
    const char* test_name = "atexit";
    print_test_header(test_name);
    int success = 1;

    int p[2];
    if (pipe(p) != 0) {
        report_failure("pipe failed");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child
        close(p[0]);
        // Перенаправляем stdout в pipe
        dup2(p[1], 1); 
        close(p[1]);

        atexit(atexit_handler);
        exit(0); // Должен вызваться handler
    } else if (pid > 0) {
        // Parent
        close(p[1]);
        char buf[10];
        int n = read(p[0], buf, sizeof(buf));
        close(p[0]);
        wait(NULL);

        // Мы ожидаем увидеть 'X' от хендлера
        if (n <= 0 || buf[0] != 'X') {
            success = 0; report_failure("atexit handler did not run");
        }
    } else {
        success = 0; report_failure("fork failed");
    }

    print_test_result(test_name, success);
}

void test_UnderscoreExit() {
    const char* test_name = "_Exit";
    print_test_header(test_name);
    int success = 1;

    int p[2];
    if (pipe(p) != 0) return;

    pid_t pid = fork();
    if (pid == 0) {
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);

        atexit(atexit_handler);
        _Exit(0); // НЕ должен вызваться handler
    } else if (pid > 0) {
        close(p[1]);
        char buf[10];
        int n = read(p[0], buf, sizeof(buf));
        close(p[0]);
        wait(NULL);

        if (n > 0 && buf[0] == 'X') {
            success = 0; report_failure("_Exit executed atexit handlers");
        }
    }

    print_test_result(test_name, success);
}

void test_rand() {
    const char* test_name = "rand/srand";
    print_test_header(test_name);
    int success = 1;

    srand(12345);
    int r1 = rand();
    srand(12345);
    int r2 = rand();

    if (r1 != r2) { success = 0; report_failure("srand reproducibility"); }
    
    // Проверка диапазона (не строгая, но базовая)
    if (r1 < 0 || r1 > RAND_MAX) { success = 0; report_failure("rand range"); }

    print_test_result(test_name, success);
}

void test_allocators() {
    const char* test_name = "malloc/realloc/calloc/free";
    print_test_header(test_name);
    int success = 1;

    void *ptr = malloc(100);
    if (!ptr) { success = 0; report_failure("malloc"); }
    
    // Calloc (check zeroing)
    int *arr = calloc(10, sizeof(int));
    if (!arr) { 
        success = 0; report_failure("calloc alloc"); 
    } else {
        for(int i=0; i<10; i++) if (arr[i] != 0) success = 0;
        if (!success) report_failure("calloc not zeroed");
    }

    // Realloc
    ptr = realloc(ptr, 200);
    if (!ptr) { success = 0; report_failure("realloc grow"); }
    
    ptr = realloc(ptr, 50);
    if (!ptr) { success = 0; report_failure("realloc shrink"); }

    free(ptr);
    free(arr);

    // realloc(NULL, size) == malloc(size)
    ptr = realloc(NULL, 20);
    if (!ptr) { success = 0; report_failure("realloc(NULL)"); }
    free(ptr);

    print_test_result(test_name, success);
}

int main() {
    print_string("========== StdLib Extended Test Suite ==========\n");

    test_abs_and_labs();
    test_div_ldiv_lldiv();
    test_basic_conversions();
    test_advanced_str_to_int();
    test_advanced_str_to_float();
    test_rand();
    test_searching_sorting();
    test_env_system();
    test_allocators();
    test_atexit();
    test_UnderscoreExit();

    print_string("================================================\n");
    if (g_failures == 0) {
        print_string("All tests passed.\n");
    } else {
        print_string("Tests finished with ");
        print_number(g_failures);
        print_string(" failure(s).\n");
    }

    return g_failures > 0 ? 1 : 0;
}
