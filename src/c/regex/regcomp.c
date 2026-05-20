#include "regex_internal.h"
#include <stdlib.h>

typedef struct {
    const char *pat;
    size_t pos;
    AstNode *prog;
    size_t len;
    size_t cap;
    int err;
    int group_id;
    int group_stack[256];
    int group_sp;
    int ret;
} Ctx;

static Ctx emit(Ctx c, AstOp op, int v1, int v2, int left, int right)
{
    if (c.err)
        return c;
    if (c.len >= c.cap) {
        size_t n = c.cap ? c.cap * 2 : 16;
        AstNode *p = (AstNode *)realloc(c.prog, n * sizeof(AstNode));
        if (!p) {
            free(c.prog);
            c.prog = NULL;
            c.err = REG_ESPACE;
            return c;
        }
        c.prog = p;
        c.cap = n;
    }
    c.prog[c.len].op = op;
    c.prog[c.len].v1 = v1;
    c.prog[c.len].v2 = v2;
    c.prog[c.len].left = left;
    c.prog[c.len].right = right;
    c.ret = ((int)c.len);
    c.len++;
    return c;
}

static Ctx copy_ast(Ctx c, int node, int *out_node)
{
    if (c.err)
        return c;
    if (node < 0) {
        *out_node = -1;
        return c;
    }
    int left = -1, right = -1;
    c = copy_ast(c, c.prog[node].left, &left);
    c = copy_ast(c, c.prog[node].right, &right);
    AstNode n = c.prog[node];
    c = emit(c, n.op, n.v1, n.v2, left, right);
    *out_node = c.ret;
    return c;
}

static int check_cls(const char *s, size_t len)
{
    const char *cls[] = {"alnum",
                         "alpha",
                         "blank",
                         "cntrl",
                         "digit",
                         "graph",
                         "lower",
                         "print",
                         "punct",
                         "space",
                         "upper",
                         "xdigit"};
    for (int i = 0; i < 12; i++) {
        size_t clen = 0;
        while (cls[i][clen])
            clen++;
        if (clen == len) {
            int match = 1;
            for (size_t j = 0; j < len; j++) {
                if (s[j] != cls[i][j]) {
                    match = 0;
                    break;
                }
            }
            if (match)
                return 1;
        }
    }
    return 0;
}

static Ctx parse_bracket(Ctx c)
{
    if (c.err)
        return c;
    size_t start = c.pos;
    c.pos++;
    if (c.pat[c.pos] == '^')
        c.pos++;
    if (c.pat[c.pos] == ']')
        c.pos++;

    int last = -1;

    while (c.pat[c.pos] != '\0' && c.pat[c.pos] != ']') {
        if (c.pat[c.pos] == '[' &&
            (c.pat[c.pos + 1] == ':' || c.pat[c.pos + 1] == '.' || c.pat[c.pos + 1] == '=')) {
            char t = c.pat[c.pos + 1];
            size_t cstart = c.pos + 2;
            c.pos += 2;
            while (c.pat[c.pos] != '\0' && !(c.pat[c.pos] == t && c.pat[c.pos + 1] == ']'))
                c.pos++;
            if (c.pat[c.pos] == '\0') {
                c.err = REG_EBRACK;
                return c;
            }
            if (t == ':') {
                if (!check_cls(c.pat + cstart, c.pos - cstart)) {
                    c.err = REG_ECTYPE;
                    return c;
                }
            } else if (t == '.' || t == '=') {
                if (c.pos - cstart == 0) {
                    c.err = REG_ECOLLATE;
                    return c;
                }
            }
            c.pos += 2;
            last = -1;
        } else if (c.pat[c.pos] == '-' && c.pat[c.pos + 1] != '\0' && c.pat[c.pos + 1] != ']' &&
                   last != -1) {
            c.pos += 2;
            last = -1;
        } else {
            last = c.pat[c.pos];
            c.pos++;
        }
    }

    if (c.pat[c.pos] != ']') {
        c.err = REG_EBRACK;
        return c;
    }

    c = emit(c, AST_BRACKET, (int)start, ((int)c.pos), -1, -1);
    c.pos++;
    return c;
}

static Ctx parse_bounds(Ctx c, int is_ere, int *out_min, int *out_max)
{
    if (c.err)
        return c;
    int min = 0, max = -1, has_min = 0;

    while (c.pat[c.pos] >= '0' && c.pat[c.pos] <= '9') {
        min = min * 10 + (c.pat[c.pos] - '0');
        has_min = 1;
        c.pos++;
    }
    if (!has_min) {
        c.err = REG_BADBR;
        return c;
    }

    if (c.pat[c.pos] == ',') {
        c.pos++;
        if (c.pat[c.pos] >= '0' && c.pat[c.pos] <= '9') {
            max = 0;
            while (c.pat[c.pos] >= '0' && c.pat[c.pos] <= '9') {
                max = max * 10 + (c.pat[c.pos] - '0');
                c.pos++;
            }
        }
    } else {
        max = min;
    }

    if (is_ere) {
        if (c.pat[c.pos] == '}')
            c.pos++;
        else {
            c.err = REG_BADBR;
            return c;
        }
    } else {
        if (c.pat[c.pos] == '\\' && c.pat[c.pos + 1] == '}')
            c.pos += 2;
        else {
            c.err = REG_BADBR;
            return c;
        }
    }

    if (min > 255 || max > 255 || (max != -1 && min > max)) {
        c.err = REG_BADBR;
        return c;
    }

    *out_min = min;
    *out_max = max;
    return c;
}

static Ctx apply_bounds(Ctx c, int node, int min, int max)
{
    if (c.err)
        return c;
    int root = -1;

    for (int i = 0; i < min; i++) {
        int copy;
        c = copy_ast(c, node, &copy);
        if (root == -1)
            root = copy;
        else
            c = emit(c, AST_CONCAT, 0, 0, root, copy);
        root = c.ret;
    }

    if (max == -1) {
        int copy;
        c = copy_ast(c, node, &copy);
        c = emit(c, AST_STAR, 0, 0, copy, -1);
        if (root == -1)
            root = c.ret;
        else
            c = emit(c, AST_CONCAT, 0, 0, root, c.ret);
        root = c.ret;
    } else {
        for (int i = min; i < max; i++) {
            int copy;
            c = copy_ast(c, node, &copy);
            c = emit(c, AST_EMPTY, 0, 0, -1, -1);
            int empty = c.ret;
            c = emit(c, AST_ALT, 0, 0, copy, empty);
            if (root == -1)
                root = c.ret;
            else
                c = emit(c, AST_CONCAT, 0, 0, root, c.ret);
            root = c.ret;
        }
    }

    if (root == -1)
        c = emit(c, AST_EMPTY, 0, 0, -1, -1);
    else
        c.ret = root;
    return c;
}

static Ctx compile_bre(Ctx c);
static Ctx compile_ere(Ctx c);

static Ctx parse_bre_escape(Ctx c)
{
    if (c.err)
        return c;
    c.pos++;
    char nx = c.pat[c.pos];
    if (nx == '\0') {
        c.err = REG_EESCAPE;
        return c;
    }
    if (nx == '(') {
        c.group_id++;
        if (c.group_sp >= 256) {
            c.err = REG_ESPACE;
            return c;
        }
        int gid = c.group_id;
        c.group_stack[c.group_sp++] = gid;
        c.pos++;
        c = compile_bre(c);
        if (c.err)
            return c;
        int inner = c.ret;
        if (c.pat[c.pos] != '\\' || c.pat[c.pos + 1] != ')') {
            c.err = REG_EPAREN;
            return c;
        }
        c.group_sp--;
        c.pos += 2;
        c = emit(c, AST_GROUP_END, gid, 0, -1, -1);
        int gend = c.ret;
        c = emit(c, AST_CONCAT, 0, 0, inner, gend);
        int concat = c.ret;
        c = emit(c, AST_GROUP, gid, 0, concat, -1);
    } else if (nx == '{') {
        c.err = REG_BADRPT;
        return c;
    } else if (nx >= '1' && nx <= '9') {
        if ((nx - '0') > c.group_id) {
            c.err = REG_ESUBREG;
            return c;
        }
        c = emit(c, AST_BACKREF, nx - '0', 0, -1, -1);
        c.pos++;
    } else {
        c = emit(c, AST_CHAR, nx, 0, -1, -1);
        c.pos++;
    }
    return c;
}

static Ctx parse_bre_atom(Ctx c)
{
    if (c.err)
        return c;
    char x = c.pat[c.pos];

    if (x == '^') {
        int is_anchor =
            (c.pos == 0) || (c.pos >= 2 && c.pat[c.pos - 1] == '(' && c.pat[c.pos - 2] == '\\');
        if (is_anchor) {
            c = emit(c, AST_BOL, 0, 0, -1, -1);
        } else {
            c = emit(c, AST_CHAR, '^', 0, -1, -1);
        }
        c.pos++;
    } else if (x == '$') {
        int is_anchor =
            (c.pat[c.pos + 1] == '\0') || (c.pat[c.pos + 1] == '\\' && c.pat[c.pos + 2] == ')');
        if (is_anchor) {
            c = emit(c, AST_EOL, 0, 0, -1, -1);
        } else {
            c = emit(c, AST_CHAR, '$', 0, -1, -1);
        }
        c.pos++;
    } else if (x == '.') {
        c = emit(c, AST_ANY, 0, 0, -1, -1);
        c.pos++;
    } else if (x == '[') {
        c = parse_bracket(c);
    } else if (x == '\\') {
        c = parse_bre_escape(c);
    } else {
        c = emit(c, AST_CHAR, x, 0, -1, -1);
        c.pos++;
    }
    return c;
}

static Ctx parse_bre_piece(Ctx c)
{
    if (c.err)
        return c;

    if (c.pat[c.pos] == '*') {
        c = emit(c, AST_CHAR, '*', 0, -1, -1);
        c.pos++;
        return c;
    }

    c = parse_bre_atom(c);
    if (c.err)
        return c;

    int node = c.ret;

    if (c.pat[c.pos] == '*') {
        c = emit(c, AST_STAR, 0, 0, node, -1);
        c.pos++;
    } else if (c.pat[c.pos] == '\\' && c.pat[c.pos + 1] == '{') {
        c.pos += 2;
        int min, max;
        c = parse_bounds(c, 0, &min, &max);
        if (!c.err)
            c = apply_bounds(c, node, min, max);
    }

    return c;
}

static Ctx compile_bre(Ctx c)
{
    int root = -1;
    while (!c.err && c.pat[c.pos] != '\0' && !(c.pat[c.pos] == '\\' && c.pat[c.pos + 1] == ')')) {
        c = parse_bre_piece(c);
        if (c.err)
            return c;
        if (root == -1)
            root = c.ret;
        else
            c = emit(c, AST_CONCAT, 0, 0, root, c.ret);
        root = c.ret;
    }
    if (root == -1)
        c = emit(c, AST_EMPTY, 0, 0, -1, -1);
    else
        c.ret = root;
    return c;
}

static Ctx parse_ere_atom(Ctx c)
{
    if (c.err)
        return c;
    char x = c.pat[c.pos];

    if (x == '*' || x == '+' || x == '?' || x == '{') {
        c.err = REG_BADRPT;
        return c;
    }

    if (x == '^') {
        c = emit(c, AST_BOL, 0, 0, -1, -1);
        c.pos++;
    } else if (x == '$') {
        c = emit(c, AST_EOL, 0, 0, -1, -1);
        c.pos++;
    } else if (x == '.') {
        c = emit(c, AST_ANY, 0, 0, -1, -1);
        c.pos++;
    } else if (x == '[') {
        c = parse_bracket(c);
    } else if (x == '(') {
        c.group_id++;
        if (c.group_sp >= 256) {
            c.err = REG_ESPACE;
            return c;
        }
        int gid = c.group_id;
        c.group_stack[c.group_sp++] = gid;
        c.pos++;
        c = compile_ere(c);
        if (c.err)
            return c;
        int inner = c.ret;
        if (c.pat[c.pos] != ')') {
            c.err = REG_EPAREN;
            return c;
        }
        c.group_sp--;
        c.pos++;
        c = emit(c, AST_GROUP_END, gid, 0, -1, -1);
        int gend = c.ret;
        c = emit(c, AST_CONCAT, 0, 0, inner, gend);
        int concat = c.ret;
        c = emit(c, AST_GROUP, gid, 0, concat, -1);
    } else if (x == '\\') {
        c.pos++;
        if (c.pat[c.pos] == '\0') {
            c.err = REG_EESCAPE;
            return c;
        }
        c = emit(c, AST_CHAR, c.pat[c.pos], 0, -1, -1);
        c.pos++;
    } else {
        c = emit(c, AST_CHAR, x, 0, -1, -1);
        c.pos++;
    }
    return c;
}

static Ctx parse_ere_piece(Ctx c)
{
    if (c.err)
        return c;
    c = parse_ere_atom(c);
    if (c.err)
        return c;

    int node = c.ret;
    char x = c.pat[c.pos];

    if (x == '*') {
        c = emit(c, AST_STAR, 0, 0, node, -1);
        c.pos++;
    } else if (x == '+') {
        int copy;
        c = copy_ast(c, node, &copy);
        c = emit(c, AST_STAR, 0, 0, copy, -1);
        c = emit(c, AST_CONCAT, 0, 0, node, c.ret);
        c.pos++;
    } else if (x == '?') {
        c = emit(c, AST_EMPTY, 0, 0, -1, -1);
        int empty = c.ret;
        c = emit(c, AST_ALT, 0, 0, node, empty);
        c.pos++;
    } else if (x == '{') {
        c.pos++;
        int min, max;
        c = parse_bounds(c, 1, &min, &max);
        if (!c.err)
            c = apply_bounds(c, node, min, max);
    }
    return c;
}

static Ctx parse_ere_branch(Ctx c)
{
    int root = -1;
    while (!c.err && c.pat[c.pos] != '\0' && c.pat[c.pos] != '|' && c.pat[c.pos] != ')') {
        c = parse_ere_piece(c);
        if (c.err)
            return c;
        if (root == -1)
            root = c.ret;
        else
            c = emit(c, AST_CONCAT, 0, 0, root, c.ret);
        root = c.ret;
    }
    if (root == -1)
        c = emit(c, AST_EMPTY, 0, 0, -1, -1);
    else
        c.ret = root;
    return c;
}

static Ctx compile_ere(Ctx c)
{
    int root = -1;
    while (!c.err && c.pat[c.pos] != '\0' && c.pat[c.pos] != ')') {
        c = parse_ere_branch(c);
        if (c.err)
            return c;
        if (root == -1)
            root = c.ret;
        else
            c = emit(c, AST_ALT, 0, 0, root, c.ret);
        root = c.ret;

        if (c.pat[c.pos] == '|')
            c.pos++;
    }
    if (root == -1)
        c = emit(c, AST_EMPTY, 0, 0, -1, -1);
    else
        c.ret = root;
    return c;
}

int regcomp(regex_t *preg, const char *pattern, int cflags)
{
    if (!preg || !pattern)
        return REG_BADPAT;

    Ctx c = {0};
    c.pat = pattern;

    if (cflags & REG_EXTENDED)
        c = compile_ere(c);
    else
        c = compile_bre(c);

    if (!c.err && c.pat[c.pos] != '\0')
        c.err = REG_EPAREN;
    if (!c.err && c.group_sp != 0)
        c.err = REG_EPAREN;

    if (c.err) {
        if (c.prog)
            free(c.prog);
        return c.err;
    }

    CompData *cd = (CompData *)malloc(sizeof(CompData));
    if (!cd) {
        free(c.prog);
        return REG_ESPACE;
    }

    cd->nodes = c.prog;
    cd->root = c.ret;
    cd->prog_len = c.len;

    size_t plen = 0;
    while (pattern[plen])
        plen++;
    cd->pat = (char *)malloc(plen + 1);
    if (!cd->pat) {
        free(c.prog);
        free(cd);
        return REG_ESPACE;
    }
    for (size_t i = 0; i <= plen; i++)
        cd->pat[i] = pattern[i];

    preg->re_comp = cd;
    preg->re_nsub = ((size_t)(c.group_id));
    preg->re_cflags = cflags;

    return 0;
}
