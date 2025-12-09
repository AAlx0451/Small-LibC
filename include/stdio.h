/*
 * Copyright (c) 2000, 2005, 2007, 2009, 2010 Apple Inc. All rights reserved.
 */

#ifndef _STDIO_H_
#define _STDIO_H_

/* Added for type compatibility after removing system includes */
#include <stddef.h> /* for size_t, NULL */
#include <stdarg.h> /* for va_list */
#include <sys/types.h> /* ott_t */

typedef off_t fpos_t;

#define _FSTDIO /* Define for new stdio with functions. */

/*
 * NB: to fit things in six character monocase externals, the stdio
 * code uses the prefix `__s' for stdio objects, typically followed
 * by a three-character attempt at a mnemonic.
 */

/* stdio buffers */
struct __sbuf {
    unsigned char *_base;
    int _size;
};

/* hold a buncha junk that would grow the ABI */
struct __sFILEX;

/*
 * stdio state variables.
 *
 * The following always hold:
 *
 *  if (_flags&(__SLBF|__SWR)) == (__SLBF|__SWR),
 *      _lbfsize is -_bf._size, else _lbfsize is 0
 *  if _flags&__SRD, _w is 0
 *  if _flags&__SWR, _r is 0
 *
 * This ensures that the getc and putc macros (or inline functions) never
 * try to write or read from a file that is in `read' or `write' mode.
 * (Moreover, they can, and do, automatically switch from read mode to
 * write mode, and back, on "r+" and "w+" files.)
 *
 * _lbfsize is used only to make the inline line-buffered output stream
 * code as compact as possible.
 *
 * _ub, _up, and _ur are used when ungetc() pushes back more characters
 * than fit in the current _bf, or when ungetc() pushes back a character
 * that does not match the previous one in _bf.  When this happens,
 * _ub._base becomes non-nil (i.e., a stream has ungetc() data iff
 * _ub._base!=NULL) and _up and _ur save the current values of _p and _r.
 *
 * NB: see WARNING above before changing the layout of this structure!
 */
typedef struct __sFILE {
    unsigned char *_p;     /* current position in (some) buffer */
    int _r;                /* read space left for getc() */
    int _w;                /* write space left for putc() */
    short _flags;          /* flags, below; this FILE is free if 0 */
    short _file;           /* fileno, if Unix descriptor, else -1 */
    struct __sbuf _bf;     /* the buffer (at least 1 byte, if !NULL) */
    int _lbfsize;          /* 0 or -_bf._size, for inline putc */

    /* operations */
    void *_cookie;         /* cookie passed to io functions */
    int (*_close)(void *);
    int (*_read)(void *, char *, int);
    fpos_t (*_seek)(void *, fpos_t, int);
    int (*_write)(void *, const char *, int);

    /* separate buffer for long sequences of ungetc() */
    struct __sbuf _ub;     /* ungetc buffer */
    struct __sFILEX *_extra; /* additions to FILE to not break ABI */
    int _ur;               /* saved _r when _r is counting ungetc data */

    /* tricks to meet minimum requirements even when malloc() fails */
    unsigned char _ubuf[3]; /* guarantee an ungetc() buffer */
    unsigned char _nbuf[1]; /* guarantee a getc() buffer */

    /* separate buffer for fgetln() when line crosses buffer boundary */
    struct __sbuf _lb;      /* buffer for fgetln() */

    /* Unix stdio files get aligned to block boundaries on fseek() */
    int _blksize;           /* stat.st_blksize (may be != _bf._size) */
    fpos_t _offset;         /* current lseek offset (see WARNING) */
} FILE;

extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;

#define __SLBF 0x0001  /* line buffered */
#define __SNBF 0x0002  /* unbuffered */
#define __SRD  0x0004  /* OK to read */
#define __SWR  0x0008  /* OK to write */
/* RD and WR are never simultaneously asserted */
#define __SRW  0x0010  /* open for reading & writing */
#define __SEOF 0x0020  /* found EOF */
#define __SERR 0x0040  /* found error */
#define __SMBF 0x0080  /* _buf is from malloc */
#define __SAPP 0x0100  /* fdopen()ed in append mode */
#define __SSTR 0x0200  /* this is an sprintf/snprintf string */
#define __SOPT 0x0400  /* do fseek() optimisation */
#define __SNPT 0x0800  /* do not do fseek() optimisation */
#define __SOFF 0x1000  /* set iff _offset is in fact correct */
#define __SMOD 0x2000  /* true => fgetln modified _p text */
#define __SALC 0x4000  /* allocate string space dynamically */
#define __SIGN 0x8000  /* ignore this file in _fwalk */

/*
 * The following three definitions are for ANSI C, which took them
 * from System V, which brilliantly took internal interface macros and
 * made them official arguments to setvbuf(), without renaming them.
 * Hence, these ugly _IOxxx names are *supposed* to appear in user code.
 *
 * Although numbered as their counterparts above, the implementation
 * does not rely on this.
 */
#define _IOFBF 0 /* setvbuf should set fully buffered */
#define _IOLBF 1 /* setvbuf should set line buffered */
#define _IONBF 2 /* setvbuf should set unbuffered */

#define BUFSIZ 1024 /* size of buffer used by setbuf */
#define EOF    (-1)

#define FOPEN_MAX    20   /* must be <= OPEN_MAX <sys/syslimits.h> */
#define FILENAME_MAX 1024 /* must be <= PATH_MAX <sys/syslimits.h> */

#define P_tmpdir     "/var/tmp/"
#define L_tmpnam     1024 /* XXX must be == PATH_MAX */
#define TMP_MAX      308915776

#ifndef SEEK_SET
#define SEEK_SET 0 /* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1 /* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define SEEK_END 2 /* set file offset to EOF plus offset */
#endif

#define stdin  __stdinp
#define stdout __stdoutp
#define stderr __stderrp

#endif /* _STDIO_H_ */
