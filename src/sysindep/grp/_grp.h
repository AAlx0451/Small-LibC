#ifndef _GRP_H
#define _GRP_H

#include <stdio.h>
#include <grp.h>

#define GROUP_FILE "/etc/group"
#define MAX_LINE_LEN 1024
#define MAX_MEMBERS 256

extern FILE *g_group_fp;
extern struct group g_group_entry;
extern char g_line_buffer[MAX_LINE_LEN];
extern char *g_members[MAX_MEMBERS];

#endif
