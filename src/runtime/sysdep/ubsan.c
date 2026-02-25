#include <stdint.h>
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

#define NO_UBSAN __attribute__((no_sanitize("undefined")))
#define NORETURN __attribute__((noreturn))
#define COLOR_BOLD_RED "\x1b[1;31m"
#define COLOR_BOLD "\x1b[1m"
#define COLOR_RESET "\x1b[0m"

struct SourceLocation {
    const char *file;
    uint32_t line;
    uint32_t column;
};

struct GenericData {
    struct SourceLocation loc;
};

struct TypeDescriptor {
    uint16_t kind;
    uint16_t info;
    char name[2];
};

struct TypeMismatchData_v1 {
    struct SourceLocation loc;
    struct TypeDescriptor *type;
    uint8_t log_alignment;
    uint8_t type_check_kind;
    uint8_t __padding[2];
};

NO_UBSAN static void raw_write_stderr(const char *buf, unsigned int len) {
    register int r0 __asm__("r0") = 2; // stderr
    register const char *r1 __asm__("r1") = buf;
    register unsigned int r2 __asm__("r2") = len;
    register int r12 __asm__("r12") = 4; // SYS_write

    __asm__ volatile(
        "svc 0x80\n"
        : "+r"(r0)
        : "r"(r1), "r"(r2), "r"(r12)
        : "memory", "cc");
}

NO_UBSAN static void print_str(const char *str) {
    unsigned int len;
    if(!str)
        str = "<unknown>";
    len = 0;
    while(str[len])
        len++;
    raw_write_stderr(str, len);
}

NO_UBSAN static unsigned int divmod10(uintptr_t *val) {
    uintptr_t q, r;
    q = (*val >> 1) + (*val >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    q = q >> 3;
    r = *val - (((q << 2) + q) << 1);
    if(r > 9) {
        q++;
        r -= 10;
    }
    *val = q;
    return r;
}

NO_UBSAN static void print_uint(uintptr_t val) {
    char buf[24];
    int i = 22;
    buf[23] = '\0';
    if(val == 0) {
        buf[i--] = '0';
    } else {
        while(val > 0) {
            /* Fix: Cast result to char to satisfy -Wconversion */
            buf[i--] = (char)('0' + divmod10(&val));
        }
    }
    print_str(&buf[i + 1]);
}

NO_UBSAN static void print_hex(uintptr_t val) {
    char buf[24];
    int i = 22;
    const char *hex_chars = "0123456789abcdef";
    buf[23] = '\0';
    if(val == 0) {
        buf[i--] = '0';
    } else {
        while(val > 0) {
            buf[i--] = hex_chars[val & 0xF];
            val >>= 4;
        }
    }
    buf[i--] = 'x';
    buf[i--] = '0';
    print_str(&buf[i + 1]);
}

NO_UBSAN static void print_loc(struct SourceLocation *loc) {
    if(!loc || !loc->file) {
        print_str(COLOR_BOLD "<unknown>:" COLOR_RESET " ");
        return;
    }

    print_str(COLOR_BOLD);
    print_str(loc->file);
    print_str(":");
    print_uint(loc->line);
    print_str(":");
    print_uint(loc->column);
    print_str(": " COLOR_RESET);
    print_str(COLOR_BOLD_RED "runtime error: " COLOR_RESET);
}

/*
 * Fixes for macros:
 * 1. Added function prototypes before definitions to satisfy -Wmissing-prototypes
 * 2. Added (void)casts to satisfy -Wunused-parameter
 */

#define UBSAN_HANDLER_3(name, msg)                                                                   \
    void __ubsan_handle_##name(void *data, uintptr_t lhs, uintptr_t rhs);                            \
    NO_UBSAN void __ubsan_handle_##name(void *data, uintptr_t lhs, uintptr_t rhs) {                  \
        (void)lhs;                                                                                   \
        (void)rhs;                                                                                   \
        print_loc(&((struct GenericData *)data)->loc);                                               \
        print_str(msg "\n");                                                                         \
    }                                                                                                \
    void __ubsan_handle_##name##_abort(void *data, uintptr_t lhs, uintptr_t rhs);                    \
    NO_UBSAN NORETURN void __ubsan_handle_##name##_abort(void *data, uintptr_t lhs, uintptr_t rhs) { \
        __ubsan_handle_##name(data, lhs, rhs);                                                       \
        __builtin_trap();                                                                            \
    }

#define UBSAN_HANDLER_2(name, msg)                                                     \
    void __ubsan_handle_##name(void *data, uintptr_t arg1);                            \
    NO_UBSAN void __ubsan_handle_##name(void *data, uintptr_t arg1) {                  \
        (void)arg1;                                                                    \
        print_loc(&((struct GenericData *)data)->loc);                                 \
        print_str(msg "\n");                                                           \
    }                                                                                  \
    void __ubsan_handle_##name##_abort(void *data, uintptr_t arg1);                    \
    NO_UBSAN NORETURN void __ubsan_handle_##name##_abort(void *data, uintptr_t arg1) { \
        __ubsan_handle_##name(data, arg1);                                             \
        __builtin_trap();                                                              \
    }

#define UBSAN_HANDLER_1(name, msg)                                     \
    void __ubsan_handle_##name(void *data);                            \
    NO_UBSAN void __ubsan_handle_##name(void *data) {                  \
        print_loc(&((struct GenericData *)data)->loc);                 \
        print_str(msg "\n");                                           \
    }                                                                  \
    void __ubsan_handle_##name##_abort(void *data);                    \
    NO_UBSAN NORETURN void __ubsan_handle_##name##_abort(void *data) { \
        __ubsan_handle_##name(data);                                   \
        __builtin_trap();                                              \
    }

UBSAN_HANDLER_3(add_overflow, "addition overflow")
UBSAN_HANDLER_3(sub_overflow, "subtraction overflow")
UBSAN_HANDLER_3(mul_overflow, "multiplication overflow")
UBSAN_HANDLER_3(divrem_overflow, "division/remainder by zero or overflow")
UBSAN_HANDLER_3(shift_out_of_bounds, "shift out of bounds")
UBSAN_HANDLER_3(pointer_overflow, "pointer overflow")

UBSAN_HANDLER_2(negate_overflow, "negation overflow")
UBSAN_HANDLER_2(out_of_bounds, "index out of bounds")
UBSAN_HANDLER_2(float_cast_overflow, "float cast overflow")
UBSAN_HANDLER_2(load_invalid_value, "load of invalid value")
UBSAN_HANDLER_2(vla_bound_not_positive, "variable length array bound is not positive")
UBSAN_HANDLER_2(nonnull_return_v1, "null pointer returned from function declared to never return null")

UBSAN_HANDLER_1(builtin_unreachable, "execution reached an unreachable program point")
UBSAN_HANDLER_1(missing_return, "execution reached the end of a value-returning function")
UBSAN_HANDLER_1(nonnull_arg, "null pointer passed as non-null argument")
UBSAN_HANDLER_1(invalid_builtin, "passing invalid arguments to a builtin")

/* Added prototypes for manual handlers */
void __ubsan_handle_type_mismatch_v1(struct TypeMismatchData_v1 *data, uintptr_t ptr);
void __ubsan_handle_type_mismatch_v1_abort(struct TypeMismatchData_v1 *data, uintptr_t ptr);

NO_UBSAN void __ubsan_handle_type_mismatch_v1(struct TypeMismatchData_v1 *data, uintptr_t ptr) {
    uintptr_t alignment = (uintptr_t)1 << data->log_alignment;

    print_loc(&data->loc);

    if(!ptr) {
        print_str("null pointer dereference\n");
    } else if(ptr & (alignment - 1)) {
        print_str("misaligned address ");
        print_hex(ptr);
        print_str(" requires ");
        print_uint(alignment);
        print_str(" byte alignment\n");
    } else {
        print_str("type mismatch (address ");
        print_hex(ptr);
        print_str(")\n");
    }
}

NO_UBSAN NORETURN void __ubsan_handle_type_mismatch_v1_abort(struct TypeMismatchData_v1 *data, uintptr_t ptr) {
    __ubsan_handle_type_mismatch_v1(data, ptr);
    __builtin_trap();
}
