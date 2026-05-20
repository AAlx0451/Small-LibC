#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wordexp.h>

struct proc_state {
    pid_t pid;
    int fd;
    FILE *stream;
};

static void await_proc(pid_t p)
{
    int st;
    do {
        if (waitpid(p, &st, 0) != -1)
            break;
    } while (errno == EINTR);
}

static char *extract_string(FILE *src)
{
    size_t cap = 64;
    size_t pos = 0;
    char *buf = malloc(cap);

    if (!buf)
        return NULL;

    while (1) {
        int val = fgetc(src);
        if (val == EOF) {
            if (pos == 0) {
                free(buf);
                return NULL;
            }
            break;
        }

        buf[pos++] = (char)val;

        if (pos == cap) {
            cap *= 2;
            char *grown = realloc(buf, cap);
            if (!grown) {
                free(buf);
                return NULL;
            }
            buf = grown;
        }

        if (val == '\0')
            break;
    }

    if (pos > 0 && buf[pos - 1] != '\0') {
        buf[pos++] = '\0';
    }

    return buf;
}

static int inspect_chars(const char *txt)
{
    int q_single = 0, q_double = 0;
    unsigned int nests = 0;

    while (*txt != '\0') {
        char current = *txt;

        if (current == '\\') {
            if (!q_single) {
                txt++;
                if (*txt == '\0')
                    return WRDE_SYNTAX;
                continue;
            }
        } else if (current == '\'') {
            if (!q_double)
                q_single = !q_single;
        } else if (current == '"') {
            if (!q_single)
                q_double = !q_double;
        }

        if (q_single) {
            txt++;
            continue;
        }

        if (current == '`')
            return WRDE_CMDSUB;

        if (current == '$' && *(txt + 1) == '(') {
            if (*(txt + 2) == '(') {
                nests += 2;
                txt += 2;
            } else {
                return WRDE_CMDSUB;
            }
        } else if (current == '(') {
            if (nests > 0)
                nests++;
            else if (!q_double)
                return WRDE_BADCHAR;
        } else if (current == ')') {
            if (nests > 0)
                nests--;
            else if (!q_double)
                return WRDE_BADCHAR;
        }

        if (!q_double && nests == 0) {
            if (current == '\n' || current == '|' || current == '&' || current == ';' ||
                current == '<' || current == '>' || current == '{' || current == '}') {
                return WRDE_BADCHAR;
            }
        }

        txt++;
    }
    return 0;
}

int wordexp(const char *restrict words, wordexp_t *restrict we, int flags)
{
    size_t base_idx = 0;
    size_t alloc_size = 0;
    char **result_vec = NULL;
    int ret_val = 0;

    if (flags & WRDE_REUSE)
        wordfree(we);

    if ((flags & WRDE_NOCMD) && (ret_val = inspect_chars(words)) != 0) {
        return ret_val;
    }

    if (flags & WRDE_APPEND) {
        base_idx = we->we_wordc;
        result_vec = we->we_wordv;
    }

    if (flags & WRDE_DOOFFS) {
        size_t limit = (size_t)(-1) / sizeof(char *);
        if (we->we_offs > (limit >> 2))
            goto err_mem;
        base_idx += we->we_offs;
    } else {
        we->we_offs = 0;
    }

    int pipes[2];
    if (pipe(pipes) != 0)
        goto err_mem;
    fcntl(pipes[0], F_SETFD, FD_CLOEXEC);
    fcntl(pipes[1], F_SETFD, FD_CLOEXEC);

    sigset_t full_mask, prev_mask;
    sigfillset(&full_mask);
    sigprocmask(SIG_BLOCK, &full_mask, &prev_mask);

    struct proc_state ps;
    ps.pid = fork();

    sigprocmask(SIG_SETMASK, &prev_mask, NULL);

    if (ps.pid < 0) {
        close(pipes[0]);
        close(pipes[1]);
        goto err_mem;
    }

    if (ps.pid == 0) {
        if (pipes[1] != STDOUT_FILENO)
            dup2(pipes[1], STDOUT_FILENO);
        else
            fcntl(STDOUT_FILENO, F_SETFD, 0);

        char payload[64];
        snprintf(payload,
                 sizeof(payload),
                 "%s \"%s %%s%c%c%c%c0 x $1 $2\"",
                 "eval",
                 "printf",
                 '\\',
                 '\\',
                 '\\',
                 '\\');

        const char *err_route = (flags & WRDE_SHOWERR) ? "" : "2>/dev/null";

        execl("/bin/sh", "sh", "-c", payload, "sh", words, err_route, (char *)NULL);
        _exit(1);
    }

    close(pipes[1]);
    ps.fd = pipes[0];
    ps.stream = fdopen(ps.fd, "r");

    if (!ps.stream) {
        close(ps.fd);
        kill(ps.pid, SIGKILL);
        await_proc(ps.pid);
        goto err_mem;
    }

    alloc_size = result_vec ? base_idx + 1 : 0;

    char *skip = extract_string(ps.stream);
    free(skip);

    if (feof(ps.stream)) {
        fclose(ps.stream);
        await_proc(ps.pid);
        return WRDE_SYNTAX;
    }

    char *token;
    while ((token = extract_string(ps.stream)) != NULL) {
        if (base_idx + 1 >= alloc_size) {
            alloc_size = alloc_size < 16 ? 16 : alloc_size * 2;
            char **expanded = realloc(result_vec, alloc_size * sizeof(char *));
            if (!expanded) {
                free(token);
                break;
            }
            result_vec = expanded;
        }
        result_vec[base_idx++] = token;
        result_vec[base_idx] = NULL;
    }

    if (!feof(ps.stream))
        ret_val = WRDE_NOSPACE;

    fclose(ps.stream);
    await_proc(ps.pid);

    if (!result_vec) {
        result_vec = calloc(base_idx + 1, sizeof(char *));
        if (!result_vec)
            ret_val = WRDE_NOSPACE;
    }

    we->we_wordv = result_vec;
    we->we_wordc = base_idx;

    if (flags & WRDE_DOOFFS) {
        if (result_vec) {
            char **wipe = result_vec;
            for (size_t n = 0; n < we->we_offs; n++)
                wipe[n] = NULL;
        }
        we->we_wordc -= we->we_offs;
    }

    return ret_val;

err_mem:
    if (!(flags & WRDE_APPEND)) {
        we->we_wordc = 0;
        we->we_wordv = NULL;
    }
    return WRDE_NOSPACE;
}
