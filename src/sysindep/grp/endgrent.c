#include "_grp.h"

void endgrent() {
    if(g_group_fp != NULL) {
        fclose(g_group_fp);
        g_group_fp = NULL;
    }
}
