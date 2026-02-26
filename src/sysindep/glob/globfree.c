#include <glob.h>
#include <stdlib.h>

void globfree(glob_t *pglob) {
    if (!pglob || !pglob->gl_pathv) return;
    
    size_t reserve = pglob->gl_offs;
    for (size_t i = 0; i < pglob->gl_pathc; i++) {
        free(pglob->gl_pathv[reserve + i]);
    }
    
    free(pglob->gl_pathv);
    pglob->gl_pathv = NULL;
    pglob->gl_pathc = 0;
}

