#include "_grp.h"

FILE *g_group_fp = NULL;

void setgrent() {
    if(g_group_fp != NULL) {
        rewind(g_group_fp);
    } else {
        g_group_fp = fopen(GROUP_FILE, "r");
    }
}
