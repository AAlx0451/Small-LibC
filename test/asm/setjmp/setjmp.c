#include <setjmp.h>
#include <stdio.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

static void dummy() {}

static int test(jmp_buf buf, int val, void (*func)())
{
    func();
    longjmp(buf, val);
}

static void e(jmp_buf buf, int val) { test(buf, val, dummy); }

static void d(jmp_buf buf, int val) { e(buf, val); }

static void c(jmp_buf buf, int val) { d(buf, val); }

static void b(jmp_buf buf, int val) { c(buf, val); }

static void a(jmp_buf buf, int val) { b(buf, val); }

void setjmp_test()
{
    jmp_buf testbuf;
    int status = setjmp(testbuf);

    if (!status)
        test(testbuf, 42, dummy);
    if (status != 42)
        printf(RED("[FAIL]") " setjmp(buf) == 42 after longjmp(buf, 42)\n");
    else
        printf(GREEN("[PASS]") " setjmp(buf) == 42 after longjmp(buf, 42)\n");

    status = setjmp(testbuf);
    if (!status)
        test(testbuf, 0, dummy);
    if (status != 1)
        printf(RED("[FAIL]") " setjmp(buf) == 1 after longjmp(buf, 0)\n");
    else
        printf(GREEN("[PASS]") " setjmp(buf) == 1 after longjmp(buf, 0)\n");

    status = setjmp(testbuf);
    if (!status)
        a(testbuf, 5);
    if (status != 5 && status != 6)
        printf(RED("[FAIL]") " setjmp(buf) == 5 after longjmp(buf, 5) in deep nesting\n");
    else if (status != 6)
        printf(GREEN("[PASS]") " setjmp(buf) == 5 after longjmp(buf, 5) in deep nesting\n");

    if (status == 5)
        a(testbuf, 6);
    if (status != 6)
        printf(RED("[FAIL]") " setjmp(buf) == 6 after longjmp(buf, 5); longjmp(buf, 6)\n");
    else
        printf(GREEN("[PASS]") " setjmp(buf) == 6 after longjmp(buf, 5); longjmp(buf, 6)\n");
}
