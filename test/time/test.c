#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// ============================================================================
// Секция с тестовым фреймворком (скопирована из вашего примера)
// ============================================================================

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

// ============================================================================
// Новые тесты для функций времени
// ============================================================================

void test_time_and_difftime() {
    const char* test_name = "time/difftime";
    print_test_header(test_name);

    int success = 1;
    time_t t1, t2, t3;

    t1 = time(NULL);
    sleep(1); // Ждем 1 секунду
    t2 = time(&t3);

    if (t2 <= t1) {
        success = 0;
        print_string("Error: time() did not advance after sleep(1).\n");
    }
    if (t2 != t3) {
        success = 0;
        print_string("Error: time() return value did not match pointer argument.\n");
    }

    double diff = difftime(t2, t1);
    // Разница должна быть >= 1.0. Используем >= для надежности на загруженных системах.
    if (diff < 1.0) {
        success = 0;
        print_string("Error: difftime() result is less than 1.0 for a 1-second interval.\n");
    }

    print_test_result(test_name, success);
}

void test_gmtime_and_asctime() {
    const char* test_name = "gmtime/asctime";
    print_test_header(test_name);

    int success = 1;
    // 946684800 секунд с начала эпохи = 1 января 2000 г., 00:00:00 UTC
    time_t cal_time = 946684800L;
    struct tm *tm_ptr = gmtime(&cal_time);

    if (tm_ptr == NULL) {
        success = 0;
        print_string("Error: gmtime() returned NULL.\n");
    } else {
        // Проверяем поля структуры tm
        if (tm_ptr->tm_sec != 0) success = 0;
        if (tm_ptr->tm_min != 0) success = 0;
        if (tm_ptr->tm_hour != 0) success = 0;
        if (tm_ptr->tm_mday != 1) success = 0;
        if (tm_ptr->tm_mon != 0) success = 0;    // Месяцы от 0 до 11
        if (tm_ptr->tm_year != 100) success = 0; // Годы с 1900
        if (tm_ptr->tm_wday != 6) success = 0;   // День недели (0=Вс, 6=Сб)

        if (!success) {
            print_string("Error: gmtime() returned incorrect tm struct values.\n");
        } else {
            // Теперь проверяем asctime
            char *time_str = asctime(tm_ptr);
            const char *expected_str = "Sat Jan  1 00:00:00 2000\n";
            if (strcmp(time_str, expected_str) != 0) {
                success = 0;
                print_string("Error: asctime() returned incorrect string.\n");
                print_string("Expected: "); print_string(expected_str);
                print_string("Got:      "); print_string(time_str);
            }
        }
    }

    print_test_result(test_name, success);
}


void test_localtime_and_mktime_roundtrip() {
    const char* test_name = "localtime/mktime roundtrip";
    print_test_header(test_name);

    int success = 1;
    time_t now, later;

    now = time(NULL);

    struct tm *local_tm_ptr = localtime(&now);
    if (local_tm_ptr == NULL) {
        success = 0;
        print_string("Error: localtime() returned NULL.\n");
    } else {
        // mktime может изменять переданную структуру, поэтому создаем копию
        struct tm local_tm_copy = *local_tm_ptr;
        later = mktime(&local_tm_copy);

        if (now != later) {
            success = 0;
            print_string("Error: mktime(localtime(t)) did not return t.\n");
        }
    }

    print_test_result(test_name, success);
}

void test_strftime() {
    const char* test_name = "strftime";
    print_test_header(test_name);
    
    int success = 1;
    char buffer[100];
    
    // Используем ту же дату, что и в тесте gmtime
    time_t cal_time = 946684800L;
    struct tm *tm_ptr = gmtime(&cal_time);

    size_t len = strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_ptr);
    const char *expected_str = "2000-01-01 00:00:00";

    if (len != strlen(expected_str)) {
        success = 0;
        print_string("Error: strftime returned incorrect length.\n");
    }

    if (strcmp(buffer, expected_str) != 0) {
        success = 0;
        print_string("Error: strftime produced incorrect string.\n");
        print_string("Expected: "); print_string(expected_str); print_string("\n");
        print_string("Got:      "); print_string(buffer); print_string("\n");
    }

    // Тест на недостаточный размер буфера
    len = strftime(buffer, 5, "%Y-%m-%d", tm_ptr);
    if (len != 0) {
        success = 0;
        print_string("Error: strftime should return 0 when buffer is too small.\n");
    }

    print_test_result(test_name, success);
}

void test_clock() {
    const char* test_name = "clock";
    print_test_header(test_name);

    int success = 1;
    clock_t start, end;

    start = clock();
    
    // Выполняем какую-нибудь работу, чтобы процессорное время увеличилось
    // volatile, чтобы компилятор не выкинул цикл
    for (volatile int i = 0; i < 2000000; ++i);
    
    end = clock();

    if (end < start) {
        success = 0;
        print_string("Error: clock() value decreased over time.\n");
    }
    // На некоторых системах с низким разрешением таймера разница может быть 0,
    // поэтому проверяем на >=. Главное, чтобы время не шло назад.
    if ((end - start) < 0) {
        success = 0;
        print_string("Error: clock() delta is negative.\n");
    }

    print_test_result(test_name, success);
}


int main() {
    print_string("========== Time Lib Test Suite ==========\n");

    test_time_and_difftime();
    test_gmtime_and_asctime();
    test_localtime_and_mktime_roundtrip();
    test_strftime();
    test_clock();

    print_string("=========================================\n");
    if (g_failures == 0) {
        print_string("All time tests passed.\n");
    } else {
        print_string("Time tests finished with ");
        print_number(g_failures);
        print_string(" failure(s).\n");
    }

    return g_failures;
}
