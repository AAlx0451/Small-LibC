/* 
 * asm
 */

void syscall_test();

// setjmp
void _longjmp_test();
void _setjmp_test();
void longjmp_test();
void setjmp_test();
void siglongjmp_test();
void sigsetjmp_test();

// signal
void sigaction_test();

// time
void gettimeofday_test();

// unistd
void fork_test();
void pipe_test();

/* 
 * c
 */

// ctype
void isalnum_test();
void isalpha_test();
void isblank_test();
void iscntrl_test();
void isdigit_test();
void isgraph_test();
void islower_test();
void isprint_test();
void ispunct_test();
void isspace_test();
void isupper_test();
void isxdigit_test();
void tolower_test();
void toupper_test();

// locale
void localeconv_test();
void setlocale_test();

// locale/unused
void runelocale_test();

// stdlib
void abs_test();

// string
void memchr_test();
void memcmp_test();
void memcpy_test();
void memmove_test();
void memset_test();
void strcat_test();
void strchr_test();
void strcmp_test();
void strcoll_test();
void strcpy_test();
void strcspn_test();
void strdup_test();
void strerror_test();
void strlen_test();
void strncat_test();
void strncmp_test();
void strncpy_test();
void strpbrk_test();
void strrchr_test();
void strspn_test();
void strstr_test();
void strtok_test();
void strxfrm_test();

// time
void asctime_test();

void (*tests[])() = {
    syscall_test, /* syscall() */
    _longjmp_test, /* _longjmp() */
    _setjmp_test, /* _setjmp() */
    longjmp_test, /* longjmp() */
    setjmp_test, /* setjmp() */
    siglongjmp_test, /* siglongjmp() */
    sigsetjmp_test, /* sigsetjmp() */
    sigaction_test, /* sigaction() */
    gettimeofday_test, /* gettimeofday() */
    fork_test, /* fork() */
    pipe_test, /* pipe() */
    isalnum_test, /* isalnum() */
    isalpha_test, /* isalpha() */
    isblank_test, /* isblank() */
    iscntrl_test, /* iscntrl() */
    isdigit_test, /* isdigit() */
    isgraph_test, /* isgraph() */
    islower_test, /* islower() */
    isprint_test, /* isprint() */
    ispunct_test, /* ispunct() */
    isspace_test, /* isspace() */
    isupper_test, /* isupper() */
    isxdigit_test, /* isxdigit() */
    tolower_test, /* tolower() */
    toupper_test, /* toupper() */
    localeconv_test, /* localeconv() */
    setlocale_test, /* setlocale() */
    runelocale_test, /* runelocale() */
    abs_test, /* abs() */
    memchr_test, /* memchr() */
    memcmp_test, /* memcmp() */
    memcpy_test, /* memcpy() */
    memmove_test, /* memmove() */
    memset_test, /* memset() */
    strcat_test, /* strcat() */
    strchr_test, /* strchr() */
    strcmp_test, /* strcmp() */
    strcoll_test, /* strcoll() */
    strcpy_test, /* strcpy() */
    strcspn_test, /* strcspn() */
    strdup_test, /* strdup() */
    strerror_test, /* strerror() */
    strlen_test, /* strlen() */
    strncat_test, /* strncat() */
    strncmp_test, /* strncmp() */
    strncpy_test, /* strncpy() */
    strpbrk_test, /* strpbrk() */
    strrchr_test, /* strrchr() */
    strspn_test, /* strspn() */
    strstr_test, /* strstr() */
    strtok_test, /* strtok() */
    strxfrm_test, /* strxfrm() */
    asctime_test, /* asctime() */
    0
};
