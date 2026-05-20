#include <sys/stat.h>
#include <unistd.h>

int remove(const char *pathname)
{
    struct stat sb;
    if (lstat(pathname, &sb) < 0) {
        return -1;
    }

    if (S_ISDIR(sb.st_mode)) {
        return rmdir(pathname);
    }

    return unlink(pathname);
}
