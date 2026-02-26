#include <glob.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

static int compare_paths(const void *a, const void *b) {
    return strcoll(*(const char **)a, *(const char **)b);
}

static int append_match(glob_t *pglob, const char *match, int flags) {
    size_t reserve = pglob->gl_offs;
    size_t new_count = pglob->gl_pathc + 1;
    char **new_pathv = realloc(pglob->gl_pathv, (reserve + new_count + 1) * sizeof(char *));
    
    if (!new_pathv) return GLOB_NOSPACE;
    pglob->gl_pathv = new_pathv;
    
    char *copy = malloc(strlen(match) + 1);
    if (!copy) return GLOB_NOSPACE;
    strcpy(copy, match);
    
    if (flags & GLOB_MARK) {
        struct stat st;
        if (stat(copy, &st) == 0 && S_ISDIR(st.st_mode)) {
            size_t len = strlen(copy);
            if (len > 0 && copy[len - 1] != '/') {
                char *tmp = malloc(len + 2);
                if (!tmp) {
                    free(copy);
                    return GLOB_NOSPACE;
                }
                strcpy(tmp, copy);
                strcat(tmp, "/");
                free(copy);
                copy = tmp;
            }
        }
    }
    
    pglob->gl_pathv[reserve + pglob->gl_pathc] = copy;
    pglob->gl_pathv[reserve + new_count] = NULL;
    pglob->gl_pathc = new_count;
    
    return 0;
}

static int glob_internal(const char *dir, const char *pattern, int flags, int (*errfunc)(const char *, int), glob_t *pglob) {
    if (!*pattern) {
        return append_match(pglob, dir, flags);
    }
    
    int noescape = (flags & GLOB_NOESCAPE);
    const char *p = pattern;
    int has_wildcard = 0;
    
    while (*p && *p != '/') {
        if (!noescape && *p == '\\' && *(p + 1)) {
            p += 2;
            continue;
        }
        if (*p == '*' || *p == '?' || *p == '[') {
            has_wildcard = 1;
        }
        p++;
    }
    
    size_t seg_len = p - pattern;
    char *segment = malloc(seg_len + 1);
    if (!segment) return GLOB_NOSPACE;
    memcpy(segment, pattern, seg_len);
    segment[seg_len] = '\0';
    
    const char *next_pattern = p;
    while (*next_pattern == '/') next_pattern++;
    int has_slash = (next_pattern > p);
    size_t slash_len = next_pattern - p;
    
    char *slashes = malloc(slash_len + 1);
    if (!slashes) { free(segment); return GLOB_NOSPACE; }
    memcpy(slashes, p, slash_len);
    slashes[slash_len] = '\0';
    
    int res = 0;
    size_t dlen = strlen(dir);
    
    if (!has_wildcard) {
        char *literal = malloc(seg_len + 1);
        if (!literal) { free(slashes); free(segment); return GLOB_NOSPACE; }
        
        char *dst = literal;
        const char *src = segment;
        while (*src) {
            if (!noescape && *src == '\\' && *(src + 1)) src++;
            *dst++ = *src++;
        }
        *dst = '\0';
        
        size_t llen = strlen(literal);
        char *new_dir = malloc(dlen + llen + slash_len + 1);
        if (!new_dir) { free(literal); free(slashes); free(segment); return GLOB_NOSPACE; }
        
        strcpy(new_dir, dir);
        strcat(new_dir, literal);
        
        struct stat st;
        const char *stat_path = new_dir[0] ? new_dir : ".";
        if ((has_slash ? stat(stat_path, &st) : lstat(stat_path, &st)) == 0) {
            if (!has_slash || S_ISDIR(st.st_mode)) {
                strcat(new_dir, slashes);
                res = glob_internal(new_dir, next_pattern, flags, errfunc, pglob);
            }
        }
        
        free(new_dir);
        free(literal);
    } else {
        const char *open_dir = (dlen == 0) ? "." : dir;
        DIR *d = opendir(open_dir);
        if (!d) {
            if (errfunc && errfunc(open_dir, errno)) res = GLOB_ABORTED;
            else if (flags & GLOB_ERR) res = GLOB_ABORTED;
        } else {
            struct dirent *dp;
            int fnm_flags = FNM_PATHNAME | FNM_PERIOD;
            if (noescape) fnm_flags |= FNM_NOESCAPE;
            
            errno = 0;
            while (!res && (dp = readdir(d)) != NULL) {
                if (fnmatch(segment, dp->d_name, fnm_flags) == 0) {
                    size_t nlen = strlen(dp->d_name);
                    char *new_dir = malloc(dlen + nlen + slash_len + 1);
                    if (!new_dir) { res = GLOB_NOSPACE; break; }
                    
                    strcpy(new_dir, dir);
                    strcat(new_dir, dp->d_name);
                    
                    if (has_slash) {
                        struct stat st;
                        if (stat(new_dir, &st) == 0 && S_ISDIR(st.st_mode)) {
                            strcat(new_dir, slashes);
                            res = glob_internal(new_dir, next_pattern, flags, errfunc, pglob);
                        }
                    } else {
                        res = glob_internal(new_dir, next_pattern, flags, errfunc, pglob);
                    }
                    free(new_dir);
                }
                errno = 0;
            }
            if (!res && errno != 0) {
                if (errfunc && errfunc(open_dir, errno)) res = GLOB_ABORTED;
                else if (flags & GLOB_ERR) res = GLOB_ABORTED;
            }
            closedir(d);
        }
    }
    
    free(slashes);
    free(segment);
    return res;
}

int glob(const char *pattern, int flags, int (*errfunc)(const char *, int), glob_t *pglob) {
    if (!(flags & GLOB_APPEND)) {
        pglob->gl_pathc = 0;
        pglob->gl_pathv = NULL;
        if (!(flags & GLOB_DOOFFS)) {
            pglob->gl_offs = 0;
        }
    }
    
    size_t reserve = (flags & GLOB_DOOFFS) ? pglob->gl_offs : 0;
    
    if (!(flags & GLOB_APPEND)) {
        pglob->gl_pathv = malloc((reserve + 1) * sizeof(char *));
        if (!pglob->gl_pathv) return GLOB_NOSPACE;
        for (size_t i = 0; i < reserve; i++) {
            pglob->gl_pathv[i] = NULL;
        }
        pglob->gl_pathv[reserve] = NULL;
    }
    
    size_t old_count = pglob->gl_pathc;
    int res = 0;
    
    const char *p = pattern;
    while (*p == '/') p++;
    size_t lead_slashes = p - pattern;
    
    if (lead_slashes > 0) {
        char *root = malloc(lead_slashes + 1);
        if (!root) return GLOB_NOSPACE;
        memcpy(root, pattern, lead_slashes);
        root[lead_slashes] = '\0';
        res = glob_internal(root, p, flags, errfunc, pglob);
        free(root);
    } else {
        res = glob_internal("", pattern, flags, errfunc, pglob);
    }
    
    if (res == 0 && pglob->gl_pathc == old_count) {
        if (flags & GLOB_NOCHECK) {
            res = append_match(pglob, pattern, flags);
        } else {
            res = GLOB_NOMATCH;
        }
    }
    
    if (res == 0 && !(flags & GLOB_NOSORT) && pglob->gl_pathc > old_count) {
        qsort(pglob->gl_pathv + reserve + old_count, pglob->gl_pathc - old_count, sizeof(char *), compare_paths);
    }
    
    return res;
}
