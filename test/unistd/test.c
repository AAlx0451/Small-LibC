#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>

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

void test_write_stdout_and_getpid() {
    const char* test_name = "write to stdout & getpid";
    print_test_header(test_name);

    int success = 1;
    const char* msg = "Testing write to stdout. PID is ";

    ssize_t written = write(1, msg, strlen(msg));
    if (written != (ssize_t)strlen(msg)) {
        success = 0;
    }

    pid_t pid = getpid();
    if (pid <= 0) {
        success = 0;
    }

    char pid_buf[20];
    itoa(pid, pid_buf);
    written = write(1, pid_buf, strlen(pid_buf));
    if (written != (ssize_t)strlen(pid_buf)) {
        success = 0;
    }

    write(1, "\n", 1);
    print_test_result(test_name, success);
}

void test_open_write_read_close() {
    const char* test_name = "open/write/read/close file";
    print_test_header(test_name);

    int success = 1;
    const char* filename = "unistd_test_file.txt";
    const char* content_to_write = "Hello, UniStd! This is a test.";
    char read_buffer[100];
    memset(read_buffer, 0, sizeof(read_buffer));

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        success = 0;
        print_string("Error: open() for writing failed.\n");
    }

    if (success) {
        ssize_t written = write(fd, content_to_write, strlen(content_to_write));
        if (written != (ssize_t)strlen(content_to_write)) {
            success = 0;
            print_string("Error: write() wrote wrong number of bytes.\n");
        }
    }

    if (fd >= 0) {
        if (close(fd) != 0) {
            success = 0;
            print_string("Error: close() after write failed.\n");
        }
    }

    if (success) {
        fd = open(filename, O_RDONLY);
        if (fd < 0) {
            success = 0;
            print_string("Error: open() for reading failed.\n");
        }
    }

    if (success) {
        ssize_t bytes_read = read(fd, read_buffer, sizeof(read_buffer) - 1);
        if (bytes_read != (ssize_t)strlen(content_to_write)) {
            success = 0;
            print_string("Error: read() read wrong number of bytes.\n");
        } else {
            if (memcmp(content_to_write, read_buffer, strlen(content_to_write)) != 0) {
                success = 0;
                print_string("Error: content mismatch after read.\n");
            }
        }
    }

    if (fd >= 0) {
        if (close(fd) != 0) {
            success = 0;
            print_string("Error: close() after read failed.\n");
        }
    }

    print_test_result(test_name, success);
}

void test_fork_wait_exit() {
    const char* test_name = "fork/wait/_exit";
    print_test_header(test_name);

    int success = 1;
    const int exit_code = 42;

    pid_t pid = fork();

    if (pid < 0) {
        success = 0;
        print_string("Error: fork() failed.\n");
    } else if (pid == 0) {
        _exit(exit_code);
        write(2, "Error: _exit() did not exit!\n", 29);
        _exit(1);
    } else {
        int status;
        pid_t waited_pid = wait(&status);

        if (waited_pid != pid) {
            success = 0;
            print_string("Error: wait() returned wrong PID.\n");
        }

        if (!WIFEXITED(status)) {
            success = 0;
            print_string("Error: child process did not terminate normally.\n");
        }

        if (WEXITSTATUS(status) != exit_code) {
            success = 0;
            print_string("Error: child process exited with wrong status code. Expected ");
            print_number(exit_code);
            print_string(", got ");
            print_number(WEXITSTATUS(status));
            print_string(".\n");
        }
    }

    print_test_result(test_name, success);
}

void test_waitpid() {
    const char* test_name = "waitpid";
    print_test_header(test_name);

    int success = 1;
    const int exit_code1 = 11;
    const int exit_code2 = 22;

    pid_t pid1 = fork();
    if (pid1 < 0) {
        success = 0;
        print_string("Error: first fork() for waitpid failed.\n");
    } else if (pid1 == 0) {
        _exit(exit_code1);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        success = 0;
        print_string("Error: second fork() for waitpid failed.\n");
    } else if (pid2 == 0) {
        _exit(exit_code2);
    }

    if (pid1 > 0 && pid2 > 0) {
        int status;
        pid_t waited_pid = waitpid(pid2, &status, 0);

        if (waited_pid != pid2) {
            success = 0;
            print_string("Error: waitpid() waited for the wrong process first.\n");
        }
        if (!WIFEXITED(status) || WEXITSTATUS(status) != exit_code2) {
            success = 0;
            print_string("Error: second child had wrong exit status.\n");
        }

        waited_pid = waitpid(pid1, &status, 0);
        if (waited_pid != pid1) {
            success = 0;
            print_string("Error: waitpid() waited for the wrong process second.\n");
        }
        if (!WIFEXITED(status) || WEXITSTATUS(status) != exit_code1) {
            success = 0;
            print_string("Error: first child had wrong exit status.\n");
        }
    }
    print_test_result(test_name, success);
}


void test_execve() {
    const char* test_name = "execve";
    print_test_header(test_name);

    int success = 1;
    pid_t pid = fork();

    if (pid < 0) {
        success = 0;
        print_string("Error: fork() for execve test failed.\n");
    } else if (pid == 0) {
        char *argv[] = {"/bin/true", NULL};
        char *envp[] = {NULL};
        execve("/bin/true", argv, envp);
        _exit(127);
    } else {
        int status;
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            success = 0;
            print_string("Error: child process after execve returned non-zero or terminated abnormally.\n");
        }
    }

    print_test_result(test_name, success);
}

void test_lseek() {
    const char* test_name = "lseek";
    print_test_header(test_name);

    int success = 1;
    const char* filename = "lseek_test_file.txt";
    const char* content = "0123456789";
    const size_t content_len = strlen(content);
    char c;
    off_t pos;

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        success = 0;
        print_string("Error: lseek test open() for writing failed.\n");
    } else {
        if (write(fd, content, content_len) != (ssize_t)content_len) {
            success = 0;
            print_string("Error: lseek test write() failed.\n");
        }
        close(fd);
    }

    if (success) {
        fd = open(filename, O_RDONLY);
        if (fd < 0) {
            success = 0;
            print_string("Error: lseek test open() for reading failed.\n");
        }
    }

    if (success) {
        pos = lseek(fd, 5, SEEK_SET);
        if (pos != 5 || read(fd, &c, 1) != 1 || c != '5') {
            success = 0;
            print_string("Error: lseek(SEEK_SET) failed.\n");
        }
    }

    if (success) {
        pos = lseek(fd, 2, SEEK_CUR);
        if (pos != 8 || read(fd, &c, 1) != 1 || c != '8') {
            success = 0;
            print_string("Error: lseek(SEEK_CUR) positive failed.\n");
        }
    }

    if (success) {
        pos = lseek(fd, -4, SEEK_CUR);
        if (pos != 5 || read(fd, &c, 1) != 1 || c != '5') {
            success = 0;
            print_string("Error: lseek(SEEK_CUR) negative failed.\n");
        }
    }

    if (success) {
        pos = lseek(fd, -1, SEEK_END);
        if (pos != (off_t)(content_len - 1) || read(fd, &c, 1) != 1 || c != '9') {
            success = 0;
            print_string("Error: lseek(SEEK_END) failed.\n");
        }
    }

    if (fd >= 0) {
        close(fd);
    }

    print_test_result(test_name, success);
}


void test_sleep_usleep() {
    const char* test_name = "sleep/usleep";
    print_test_header(test_name);

    int success = 1;

    print_string("Info: testing sleep(1), please wait...\n");
    if (sleep(1) != 0) {
        success = 0;
        print_string("Error: sleep(1) returned a non-zero value.\n");
    }

    print_string("Info: testing usleep(10000)...\n");
    if (usleep(10000) != 0) {
        success = 0;
        print_string("Error: usleep(10000) returned a non-zero value.\n");
    }

    print_test_result(test_name, success);
}

void test_sbrk() {
    const char* test_name = "sbrk";
    print_test_header(test_name);

    int success = 1;
    const intptr_t increment = 1024;

    void* initial_brk = sbrk(0);
    if (initial_brk == (void*)-1) {
        success = 0;
        print_string("Error: initial sbrk(0) failed.\n");
    }

    if (success) {
        void* old_brk = sbrk(increment);
        if (old_brk != initial_brk) {
            success = 0;
            print_string("Error: sbrk(increment) returned wrong previous break.\n");
        }
    }

    if (success) {
        void* new_brk = sbrk(0);
        if (new_brk != (void*)((char*)initial_brk + increment)) {
            success = 0;
            print_string("Error: program break not incremented correctly.\n");
        }
    }

    if (success) {
        sbrk(-increment);
        void* final_brk = sbrk(0);
        if (final_brk != initial_brk) {
            success = 0;
            print_string("Error: program break not restored correctly.\n");
        }
    }

    print_test_result(test_name, success);
}

void test_getrusage() {
    const char* test_name = "getrusage";
    print_test_header(test_name);
    
    int success = 1;
    struct rusage usage_before, usage_after;

    if (getrusage(RUSAGE_SELF, &usage_before) != 0) {
        success = 0;
        print_string("Error: initial getrusage() call failed.\n");
    }
    
    for (volatile int i = 0; i < 2000000; ++i);

    if (getrusage(RUSAGE_SELF, &usage_after) != 0) {
        success = 0;
        print_string("Error: second getrusage() call failed.\n");
    }

    if (success) {
        long usec_before = usage_before.ru_utime.tv_sec * 1000000 + usage_before.ru_utime.tv_usec;
        long usec_after = usage_after.ru_utime.tv_sec * 1000000 + usage_after.ru_utime.tv_usec;
        if (usec_after < usec_before) {
            success = 0;
            print_string("Error: user time decreased after work.\n");
        }
    }

    print_test_result(test_name, success);
}

int main() {
    print_string("========== Unistd Test Suite ==========\n");

    test_write_stdout_and_getpid();
    test_open_write_read_close();
    test_lseek();
    test_sbrk();
    test_fork_wait_exit();
    test_waitpid();
    test_execve();
    test_sleep_usleep();
    test_getrusage();

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
