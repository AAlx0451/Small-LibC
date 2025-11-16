#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>


ssize_t read(int fd, void *buf, size_t count) {
  long ret = syscall(SYS_read, fd, buf, count);
  if (ret < 0) {
    return -1;
  }
  return (ssize_t)ret;
}
