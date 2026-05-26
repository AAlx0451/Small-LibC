void stack_chk_test();
void __divdi3_test();
void __divsi3_test();
void __fixdfdi_test();
void _c_startup_test();
void __fixunsdfdi_test();
void __floatdidf_test();
void __floatundidf_test();
void __moddi3_test();
void __modsi3_test();
void __udivdi3_test();
void __udivsi3_test();
void __umoddi3_test();
void __umodsi3_test();

void (*tests[])() = {_c_startup_test,    /* crt0.o */
                     stack_chk_test,     /* stack smashing protection */
                     __divdi3_test,      /* long long division */
                     __divsi3_test,      /* int division */
                     __fixdfdi_test,     /* double to long long casting */
                     __fixunsdfdi_test,  /* double to unsigned long long casting */
                     __floatdidf_test,   /* long long to double casting */
                     __floatundidf_test, /* unsigned long long to double casting */
                     __moddi3_test,      /* long long modulo */
                     __modsi3_test,      /* int modulo */
                     __udivdi3_test,     /* unsigned long long division */
                     __udivsi3_test,     /* unsigned int division */
                     __umoddi3_test,     /* unsigned long long modulo */
                     __umodsi3_test,     /* unsigned int modulo */
                     0};
