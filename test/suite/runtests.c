#include "tests.h"

int main()
{
    for (int i = 0; tests[i]; i++)
        tests[i]();
}
