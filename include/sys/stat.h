#ifndef SYS_STAT_H
#define SYS_STAT_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

/* macros */

// file type masks
#define S_IFMT 0170000
#define S_IFIFO 0010000
#define S_IFCHR 0020000
#define S_IFDIR 0040000
#define S_IFBLK 0060000
#define S_IFREG 0100000

#if (_POSIX_C_SOURCE >= 200112L) || defined(_DARWIN_C_SOURCE) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600)
# define S_IFLNK 0120000
# define S_IFSOCK 0140000
#endif /* (_POSIX_C_SOURCE >= 200112L) || _DARWIN_C_SOURCE || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600) */

// file type macros
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK)
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)

#if (_POSIX_C_SOURCE >= 200112L) || defined(_DARWIN_C_SOURCE) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600)
#define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
#endif /* (_POSIX_C_SOURCE >= 200112L) || _DARWIN_C_SOURCE || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600) */

// file mode bits
#define S_ISUID 0004000
#define S_ISGID 0002000
#define S_ISVTX 0001000
#define S_IRWXU 0000700
#define S_IRUSR 0000400
#define S_IWUSR 0000200
#define S_IXUSR 0000100
#define S_IRWXG 0000070
#define S_IRGRP 0000040
#define S_IWGRP 0000020
#define S_IXGRP 0000010
#define S_IRWXO 0000007
#define S_IROTH 0000004
#define S_IWOTH 0000002
#define S_IXOTH 0000001

/* definitions */
struct stat
{
    dev_t st_dev;
    ino_t st_ino;
    mode_t st_mode;
    nlink_t st_nlink;
    uid_t st_uid;
    gid_t st_gid;
    dev_t st_rdev;
    time_t st_atime;
    long st_atimensec;
    time_t st_mtime;
    long st_mtimensec;
    time_t st_ctime;
    long st_ctimensec;
    off_t st_size;
    blkcnt_t st_blocks;
    blksize_t st_blksize;
    uint32_t st_flags;
    uint32_t st_gen;
    int32_t st_lspare;
    int64_t st_qspare[2];
};

/* signatures */

// *stat
int stat(const char *path, struct stat *sb);
int fstat(int fd, struct stat *sb);

#if (_POSIX_C_SOURCE >= 200112L) || defined(_DARWIN_C_SOURCE) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600)
int lstat(const char *path, struct stat *sb);
#endif /* (_POSIX_C_SOURCE >= 200112L) || _DARWIN_C_SOURCE || (_XOPEN_SOURCE && _XOPEN_SOURCE >= 600) */

// file ops
int mkdir(const char *path, mode_t mode);
int chmod(const char *path, mode_t mode);
mode_t umask(mode_t mask);

// ipc
int mkfifo(const char *path, mode_t mode);

#ifdef _XOPEN_SOURCE
int mknod(const char *path, mode_t mode, dev_t dev);
#endif /* _XOPEN_SOURCE */

#endif /* !_ANSI && (_POSIX_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !SYS_STAT_H */
