#include "impl.h"
#include <fenv.h>

int feupdateenv(const fenv_t *envp) {
    /* save all exceptions */
    int currently_raised = fetestexcept(FE_ALL_EXCEPT);

    fesetenv(envp);

    /* restore all exceptions */
    feraiseexcept(currently_raised);
    return 0;
}
