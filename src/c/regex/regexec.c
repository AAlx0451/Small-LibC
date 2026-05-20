#include "regex_internal.h"
#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    I_MATCH,
    I_CHAR,
    I_ANY,
    I_BOL,
    I_EOL,
    I_BRACKET,
    I_BACKREF,
    I_SPLIT,
    I_JMP,
    I_SAVE
} Opcode;

typedef struct {
    Opcode op;
    int v1;
    int v2;
    int out1;
    int out2;
} NfaInst;

typedef struct {
    AstNode *prog;
    const char *str;
    size_t len;
    int cflags;
    int eflags;
    regmatch_t *best_m;
    regoff_t best_len;
    size_t internal_nmatch;
    const char *pat;
    size_t start_pos;
    int err;

    NfaInst *nfa;
    int nfa_len;
    int nfa_cap;
    int *handled; // For state deduplication
} ExecCtx;

typedef struct {
    int pc;
    int bref_id;
    int bref_offset;
    regoff_t *saves;
} Thread;

typedef struct {
    Thread *t;
    int len;
    int cap;
    regoff_t *saves_pool;
} ThreadList;

static int new_inst(ExecCtx *ex, Opcode op, int v1, int v2, int out1, int out2)
{
    if (ex->err)
        return -1;
    if (ex->nfa_len >= ex->nfa_cap) {
        int n = ex->nfa_cap ? ex->nfa_cap * 2 : 64;
        NfaInst *p = (NfaInst *)realloc(ex->nfa, (unsigned)n * sizeof(NfaInst));
        if (!p) {
            ex->err = REG_ESPACE;
            return -1;
        }
        ex->nfa = p;
        ex->nfa_cap = n;
    }
    int id = ex->nfa_len++;
    ex->nfa[id].op = op;
    ex->nfa[id].v1 = v1;
    ex->nfa[id].v2 = v2;
    ex->nfa[id].out1 = out1;
    ex->nfa[id].out2 = out2;
    return id;
}

static int build_nfa(ExecCtx *ex, int node, int out)
{
    if (ex->err)
        return -1;
    if (node < 0)
        return out;

    AstNode *n = &ex->prog[node];
    switch (n->op) {
    case AST_EMPTY:
        return out;
    case AST_CHAR:
        return new_inst(ex, I_CHAR, n->v1, 0, out, -1);
    case AST_ANY:
        return new_inst(ex, I_ANY, 0, 0, out, -1);
    case AST_BOL:
        return new_inst(ex, I_BOL, 0, 0, out, -1);
    case AST_EOL:
        return new_inst(ex, I_EOL, 0, 0, out, -1);
    case AST_BRACKET:
        return new_inst(ex, I_BRACKET, n->v1, n->v2, out, -1);
    case AST_BACKREF:
        return new_inst(ex, I_BACKREF, n->v1, 0, out, -1);
    case AST_CONCAT: {
        int r = build_nfa(ex, n->right, out);
        int l = build_nfa(ex, n->left, r);
        return l;
    }
    case AST_ALT: {
        int l = build_nfa(ex, n->left, out);
        int r = build_nfa(ex, n->right, out);
        return new_inst(ex, I_SPLIT, 0, 0, l, r);
    }
    case AST_STAR: {
        int split = new_inst(ex, I_SPLIT, 0, 0, -1, out);
        int body = build_nfa(ex, n->left, split);
        if (ex->err)
            return -1;
        ex->nfa[split].out1 = body;
        return split;
    }
    case AST_GROUP: {
        int in = build_nfa(ex, n->left, out);
        return new_inst(ex, I_SAVE, n->v1 * 2, 0, in, -1);
    }
    case AST_GROUP_END: {
        return new_inst(ex, I_SAVE, n->v1 * 2 + 1, 0, out, -1);
    }
    }
    return out;
}

static int eval_bracket(ExecCtx *ex, int start, int end, char c)
{
    int invert = 0;
    int p = start + 1;
    if (ex->pat[p] == '^') {
        invert = 1;
        p++;
    }

    if (invert && c == '\n' && (ex->cflags & REG_NEWLINE))
        return 0;

    int match = 0;
    int last = -1;

    while (p < end) {
        if (ex->pat[p] == '[' &&
            (ex->pat[p + 1] == ':' || ex->pat[p + 1] == '.' || ex->pat[p + 1] == '=')) {
            char type = ex->pat[p + 1];
            p += 2;
            int cstart = p;
            while (ex->pat[p] != type || ex->pat[p + 1] != ']')
                p++;
            size_t clen = ((size_t)(p - cstart));
            char buf[256] = {0};
            for (size_t i = 0; i < clen && i < 255; i++)
                buf[i] = ex->pat[(unsigned)cstart + i];

            if (type == ':') {
                if (strcmp(buf, "alnum") == 0 && isalnum((unsigned char)c))
                    match = 1;
                else if (strcmp(buf, "alpha") == 0 && isalpha((unsigned char)c))
                    match = 1;
                else if (strcmp(buf, "blank") == 0 && (c == ' ' || c == '\t'))
                    match = 1;
                else if (strcmp(buf, "cntrl") == 0 && iscntrl((unsigned char)c))
                    match = 1;
                else if (strcmp(buf, "digit") == 0 && isdigit((unsigned char)c))
                    match = 1;
                else if (strcmp(buf, "graph") == 0 && isgraph((unsigned char)c))
                    match = 1;
                else if (strcmp(buf, "lower") == 0 && islower((unsigned char)c))
                    match = 1;
                else if (strcmp(buf, "print") == 0 && isprint((unsigned char)c))
                    match = 1;
                else if (strcmp(buf, "punct") == 0 && ispunct((unsigned char)c))
                    match = 1;
                else if (strcmp(buf, "space") == 0 && isspace((unsigned char)c))
                    match = 1;
                else if (strcmp(buf, "upper") == 0 && isupper((unsigned char)c))
                    match = 1;
                else if (strcmp(buf, "xdigit") == 0 && isxdigit((unsigned char)c))
                    match = 1;
            } else if (type == '=') {
                char s_c[2] = {c, 0};
                if (strcoll(buf, s_c) == 0)
                    match = 1;
            } else if (type == '.') {
                if (clen == 1 && buf[0] == c)
                    match = 1;
            }
            p += 2;
            last = -1;
        } else if (ex->pat[p] == '-' && p + 1 < end && last != -1) {
            char endc = ex->pat[p + 1];
            char s_start[2] = {(char)last, 0};
            char s_end[2] = {endc, 0};
            char s_c[2] = {c, 0};
            if (ex->cflags & REG_ICASE) {
                s_start[0] = (char)tolower((unsigned char)s_start[0]);
                s_end[0] = (char)tolower((unsigned char)s_end[0]);
                s_c[0] = (char)tolower((unsigned char)s_c[0]);
            }
            if (strcoll(s_start, s_c) <= 0 && strcoll(s_c, s_end) <= 0)
                match = 1;
            p += 2;
            last = -1;
        } else {
            char check_c = c;
            char check_pat = ex->pat[p];
            if (ex->cflags & REG_ICASE) {
                check_c = (char)tolower((unsigned char)check_c);
                check_pat = (char)tolower((unsigned char)check_pat);
            }
            if (check_c == check_pat)
                match = 1;
            last = ex->pat[p];
            p++;
        }
    }
    return invert ? !match : match;
}

static void push_thread(ThreadList *l,
                        int pc,
                        regoff_t *saves,
                        int bref_id,
                        int bref_offset,
                        size_t nmatch)
{
    if (l->len >= l->cap)
        return;
    Thread *th = &l->t[l->len];
    th->pc = pc;
    th->bref_id = bref_id;
    th->bref_offset = bref_offset;
    th->saves = &l->saves_pool[((unsigned)(l->len)) * nmatch * 2];
    for (size_t i = 0; i < nmatch * 2; i++)
        th->saves[i] = saves[i];
    l->len++;
}

static void add_thread(ExecCtx *ex,
                       ThreadList *l,
                       int pc,
                       regoff_t *saves,
                       int bref_id,
                       int bref_offset,
                       size_t sp,
                       int step_id)
{
    if (ex->err || pc < 0)
        return;

    if (ex->handled[pc] == step_id)
        return;
    ex->handled[pc] = step_id;

    NfaInst *inst = &ex->nfa[pc];

    switch (inst->op) {
    case I_MATCH: {
        regoff_t cur_len = (regoff_t)(sp - ex->start_pos);
        if (cur_len > ex->best_len) {
            ex->best_len = cur_len;
            if (ex->internal_nmatch > 0 && ex->best_m) {
                ex->best_m[0].rm_so = (regoff_t)ex->start_pos;
                ex->best_m[0].rm_eo = (regoff_t)sp;
                for (size_t i = 1; i < ex->internal_nmatch; i++) {
                    ex->best_m[i].rm_so = saves[i * 2];
                    ex->best_m[i].rm_eo = saves[i * 2 + 1];
                }
            }
        }
        break;
    }
    case I_JMP:
        add_thread(ex, l, inst->out1, saves, bref_id, bref_offset, sp, step_id);
        break;
    case I_SPLIT:
        add_thread(ex, l, inst->out1, saves, bref_id, bref_offset, sp, step_id);
        add_thread(ex, l, inst->out2, saves, bref_id, bref_offset, sp, step_id);
        break;
    case I_SAVE: {
        int idx = inst->v1;
        regoff_t old = saves[idx];
        saves[idx] = (regoff_t)sp;
        add_thread(ex, l, inst->out1, saves, bref_id, bref_offset, sp, step_id);
        saves[idx] = old; // Restore for fallback branches
        break;
    }
    case I_BOL:
        if (sp == 0) {
            if (!(ex->eflags & REG_NOTBOL))
                add_thread(ex, l, inst->out1, saves, bref_id, bref_offset, sp, step_id);
        } else if ((ex->cflags & REG_NEWLINE) && ex->str[sp - 1] == '\n') {
            add_thread(ex, l, inst->out1, saves, bref_id, bref_offset, sp, step_id);
        }
        break;
    case I_EOL:
        if (sp == ex->len) {
            if (!(ex->eflags & REG_NOTEOL))
                add_thread(ex, l, inst->out1, saves, bref_id, bref_offset, sp, step_id);
        } else if ((ex->cflags & REG_NEWLINE) && sp < ex->len && ex->str[sp] == '\n') {
            add_thread(ex, l, inst->out1, saves, bref_id, bref_offset, sp, step_id);
        }
        break;
    case I_BACKREF: {
        regoff_t b_so = saves[inst->v1 * 2];
        regoff_t b_eo = saves[inst->v1 * 2 + 1];
        if (b_so != -1 && b_eo != -1) {
            if (b_eo - b_so == 0) {
                add_thread(ex, l, inst->out1, saves, bref_id, bref_offset, sp, step_id);
            } else {
                push_thread(l, pc, saves, inst->v1, 0, ex->internal_nmatch);
            }
        }
        break;
    }
    case I_CHAR:
    case I_ANY:
    case I_BRACKET:
        // Opcodes that consume a character are stored to wait for the next string step
        push_thread(l, pc, saves, bref_id, bref_offset, ex->internal_nmatch);
        break;
    }
}

static int cleanup(ExecCtx *ex,
                   ThreadList *clist,
                   ThreadList *nlist,
                   regoff_t *init_saves,
                   regmatch_t *best_m,
                   int ret)
{
    if (ex->nfa)
        free(ex->nfa);
    if (ex->handled)
        free(ex->handled);
    if (clist->t)
        free(clist->t);
    if (clist->saves_pool)
        free(clist->saves_pool);
    if (nlist->t)
        free(nlist->t);
    if (nlist->saves_pool)
        free(nlist->saves_pool);
    if (init_saves)
        free(init_saves);
    if (best_m)
        free(best_m);
    return ret;
}

int regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags)
{
    if (!preg || !string || !preg->re_comp)
        return REG_BADPAT;

    CompData *cd = (CompData *)preg->re_comp;

    size_t slen = 0;
    while (string[slen])
        slen++;

    ExecCtx ex = {0};
    ex.prog = cd->nodes;
    ex.pat = cd->pat;
    ex.str = string;
    ex.len = slen;
    ex.cflags = preg->re_cflags;
    ex.eflags = eflags;
    ex.internal_nmatch = preg->re_nsub + 1;

    // Compile AST down to NFA
    int match_pc = new_inst(&ex, I_MATCH, 0, 0, -1, -1);
    int root_pc = build_nfa(&ex, cd->root, match_pc);
    if (ex.err)
        return cleanup(&ex, NULL, NULL, NULL, NULL, ex.err);

    ex.handled = (int *)calloc(((unsigned)(ex.nfa_len)), sizeof(int));
    if (!ex.handled)
        return cleanup(&ex, NULL, NULL, NULL, NULL, REG_ESPACE);

    ThreadList clist = {0}, nlist = {0};
    clist.cap = ex.nfa_len;
    clist.t = (Thread *)malloc(((size_t)(ex.nfa_len)) * sizeof(Thread));
    clist.saves_pool =
        (regoff_t *)malloc(((size_t)(ex.nfa_len)) * ex.internal_nmatch * 2 * sizeof(regoff_t));
    nlist.cap = ex.nfa_len;
    nlist.t = (Thread *)malloc(((size_t)(ex.nfa_len)) * sizeof(Thread));
    nlist.saves_pool =
        (regoff_t *)malloc(((size_t)(ex.nfa_len)) * ex.internal_nmatch * 2 * sizeof(regoff_t));

    regoff_t *init_saves = (regoff_t *)calloc(ex.internal_nmatch * 2, sizeof(regoff_t));
    regmatch_t *best_m = NULL;

    if (!(preg->re_cflags & REG_NOSUB) && ex.internal_nmatch > 0) {
        best_m = (regmatch_t *)malloc(ex.internal_nmatch * sizeof(regmatch_t));
        ex.best_m = best_m;
    }

    if (!clist.t || !clist.saves_pool || !nlist.t || !nlist.saves_pool || !init_saves ||
        (!(preg->re_cflags & REG_NOSUB) && ex.internal_nmatch > 0 && !best_m)) {
        return cleanup(&ex, &clist, &nlist, init_saves, best_m, REG_ESPACE);
    }

    for (size_t i = 0; i < ex.internal_nmatch * 2; i++)
        init_saves[i] = -1;

    int step_id = 0;

    for (size_t start = 0; start <= slen; start++) {
        ex.best_len = -1;
        ex.start_pos = start;

        if (best_m) {
            for (size_t i = 0; i < ex.internal_nmatch; i++) {
                best_m[i].rm_so = -1;
                best_m[i].rm_eo = -1;
            }
        }

        clist.len = 0;
        step_id++;

        // Prevent integer overflow on handled generation IDs
        if (step_id >= 2000000000) {
            memset(ex.handled, 0, ((unsigned)(ex.nfa_len)) * sizeof(int));
            step_id = 1;
        }

        add_thread(&ex, &clist, root_pc, init_saves, -1, 0, start, step_id);

        for (size_t sp = start; sp <= slen; sp++) {
            if (clist.len == 0)
                break;
            if (ex.err)
                return cleanup(&ex, &clist, &nlist, init_saves, best_m, ex.err);

            char c = (sp < slen) ? ex.str[sp] : '\0';
            nlist.len = 0;
            step_id++;

            for (int i = 0; i < clist.len; i++) {
                Thread *th = &clist.t[i];
                NfaInst *inst = &ex.nfa[th->pc];

                // Active Backreference reading
                if (th->bref_id != -1) {
                    regoff_t b_so = th->saves[th->bref_id * 2];
                    regoff_t b_eo = th->saves[th->bref_id * 2 + 1];
                    regoff_t b_len = b_eo - b_so;
                    if (sp < slen) {
                        char c1 = c;
                        char c2 = ex.str[b_so + th->bref_offset];
                        if (ex.cflags & REG_ICASE) {
                            c1 = (char)tolower((unsigned char)c1);
                            c2 = (char)tolower((unsigned char)c2);
                        }
                        if (c1 == c2) {
                            if (th->bref_offset + 1 == b_len) {
                                add_thread(
                                    &ex, &nlist, inst->out1, th->saves, -1, 0, sp + 1, step_id);
                            } else {
                                push_thread(&nlist,
                                            th->pc,
                                            th->saves,
                                            th->bref_id,
                                            th->bref_offset + 1,
                                            ex.internal_nmatch);
                            }
                        }
                    }
                    continue;
                }

                // Normal Character Matching
                if (sp < slen) {
                    int matched = 0;
                    if (inst->op == I_CHAR) {
                        char c1 = c;
                        char c2 = ((char)(inst->v1));
                        if (ex.cflags & REG_ICASE) {
                            c1 = (char)tolower((unsigned char)c1);
                            c2 = (char)tolower((unsigned char)c2);
                        }
                        matched = (c1 == c2);
                    } else if (inst->op == I_ANY) {
                        matched = !((ex.cflags & REG_NEWLINE) && c == '\n');
                    } else if (inst->op == I_BRACKET) {
                        matched = eval_bracket(&ex, inst->v1, inst->v2, c);
                    }

                    if (matched) {
                        add_thread(&ex, &nlist, inst->out1, th->saves, -1, 0, sp + 1, step_id);
                    }
                }
            }

            // Swap working thread lists
            ThreadList tmp = clist;
            clist = nlist;
            nlist = tmp;
        }

        if (ex.best_len != -1) {
            if (!(preg->re_cflags & REG_NOSUB) && nmatch > 0 && pmatch) {
                size_t copy_n = nmatch < ex.internal_nmatch ? nmatch : ex.internal_nmatch;
                for (size_t i = 0; i < copy_n; i++)
                    pmatch[i] = best_m[i];
                for (size_t i = copy_n; i < nmatch; i++) {
                    pmatch[i].rm_so = -1;
                    pmatch[i].rm_eo = -1;
                }
            }
            return cleanup(&ex, &clist, &nlist, init_saves, best_m, 0);
        }
    }

    return cleanup(&ex, &clist, &nlist, init_saves, best_m, REG_NOMATCH);
}
