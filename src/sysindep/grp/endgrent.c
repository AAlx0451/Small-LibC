#include "_grp.h"

void endgrent(void) {
    if(g_group_fp != NULL) {
        fclose(g_group_fp);
        g_group_fp = NULL;
    }
}
