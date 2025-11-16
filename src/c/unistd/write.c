#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

ssize_t write(int fd, const void *buf, size_t count) {
  long ret = syscall(SYS_write, fd, buf, count);
  if (ret < 0) {
    return -1;
  }
  return (ssize_t)ret;
}

