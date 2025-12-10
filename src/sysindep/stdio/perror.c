#include <unistd.h>
#include <string.h>
#include <errno.h>

void perror(const char *s)
{
    const char *error_msg = strerror(errno);
    if (s != NULL && *s != '\0')
    {
        write(STDERR_FILENO, s, strlen(s));
        write(STDERR_FILENO, ": ", 2);
    }
    write(STDERR_FILENO, error_msg, strlen(error_msg));
    write(STDERR_FILENO, "\n", 1);
}
