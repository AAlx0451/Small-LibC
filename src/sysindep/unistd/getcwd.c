#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

char *getcwd(char *buf, size_t size) {
    char temp_buf[PATH_MAX];
    char *ptr;
    struct stat st_cur;
    struct stat st_root;
    struct stat st_parent;
    DIR *dir;
    struct dirent *entry;
    int found;

    if(!buf || !size) {
        errno = EINVAL;
        return NULL;
    }

    if(stat(".", &st_cur) == -1)
        return NULL;
    if(stat("/", &st_root) == -1)
        return NULL;

    ptr = temp_buf + sizeof(temp_buf) - 1;
    *ptr = '\0';

    if(st_cur.st_dev == st_root.st_dev && st_cur.st_ino == st_root.st_ino) {
        *--ptr = '/';
    } else {
        while(1) {
            if(st_cur.st_dev == st_root.st_dev && st_cur.st_ino == st_root.st_ino) {
                break;
            }

            if(stat("..", &st_parent) == -1)
                return NULL;
            if(chdir("..") == -1)
                return NULL;

            if((dir = opendir(".")) == NULL)
                return NULL;

            found = 0;
            while((entry = readdir(dir)) != NULL) {
                if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                    continue;
                }

                struct stat st_child;
                if(lstat(entry->d_name, &st_child) == -1) {
                    continue;
                }

                if(st_child.st_dev == st_cur.st_dev && st_child.st_ino == st_cur.st_ino) {
                    size_t len = strlen(entry->d_name);
                    if((ptr - len - 1) < temp_buf) {
                        closedir(dir);
                        errno = ERANGE;
                        return NULL;
                    }
                    ptr -= len;
                    memcpy(ptr, entry->d_name, len);
                    *--ptr = '/';
                    found = 1;
                    break;
                }
            }
            closedir(dir);

            if(!found) {
                errno = ENOENT;
                return NULL;
            }

            st_cur = st_parent;
        }
    }

    size_t res_len = ((size_t)temp_buf + sizeof(temp_buf) - 1U) - (size_t)ptr;
    if(res_len >= size) {
        errno = ERANGE;
        chdir(ptr);
        return NULL;
    }

    memcpy(buf, ptr, res_len + 1);

    if(chdir(buf) == -1) {
        return NULL;
    }

    return buf;
}
