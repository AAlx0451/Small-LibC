#include "_grp.h"
#include <stdlib.h>
#include <string.h>

struct group g_group_entry;
char g_line_buffer[MAX_LINE_LEN];
char *g_members[MAX_MEMBERS];

static struct group *parse_line(char *line) {
    char *p;
    char *members_field;
    int i = 0;

    if(line[0] == '#' || line[0] == '\n') {
        return NULL;
    }

    p = strtok(line, ":\n");
    if(p == NULL)
        return NULL;
    g_group_entry.gr_name = p;

    p = strtok(NULL, ":\n");
    if(p == NULL)
        return NULL;
    g_group_entry.gr_passwd = p;

    p = strtok(NULL, ":\n");
    if(p == NULL)
        return NULL;
    g_group_entry.gr_gid = (gid_t)atol(p);

    members_field = strtok(NULL, "\n");
    g_group_entry.gr_mem = g_members;

    if(members_field != NULL) {
        p = strtok(members_field, ",");
        while(p != NULL && i < MAX_MEMBERS - 1) {
            g_members[i++] = p;
            p = strtok(NULL, ",");
        }
    }

    g_members[i] = NULL;

    return &g_group_entry;
}

struct group *getgrent(void) {
    if(g_group_fp == NULL) {
        setgrent();
        if(g_group_fp == NULL) {
            return NULL;
        }
    }

    while(fgets(g_line_buffer, sizeof(g_line_buffer), g_group_fp) != NULL) {
        struct group *grp = parse_line(g_line_buffer);
        if(grp != NULL) {
            return grp;
        }
    }

    return NULL;
}
