#ifndef H_PERLIO
#define H_PERLIO 1

/* Clean up (or at least document) the various possible #defines.
   This section attempts to match the 5.003_03 Configure variables
   onto the 5.003_02 header file values.
   I can't figure out where USE_STDIO was supposed to be set.
   --AD
*/
#ifndef USE_PERLIO
# define PERLIO_IS_STDIO
#endif

/* Below is the 5.003_02 stuff. */
#ifdef USE_STDIO
#  ifndef PERLIO_IS_STDIO
#      define PERLIO_IS_STDIO
#  endif
#else
extern void PerlIO_init _((void));
#endif

#ifdef PERL_OBJECT

#include "ipstdio.h"

#define PerlIO_canset_cnt(f)	1
#define PerlIO_has_base(f)		1
#define PerlIO_has_cntptr(f)	1
#define PerlIO_fast_gets(f)		1

#define PerlIO_stdin()			piStdIO->Stdin()
#define PerlIO_stdout()			piStdIO->Stdout()
#define PerlIO_stderr()			piStdIO->Stderr()
#define PerlIO_open(x,y)		piStdIO->Open((x),(y), ErrorNo())
#define PerlIO_close(f)			piStdIO->Close((f), ErrorNo())
#define PerlIO_eof(f)			piStdIO->Eof((f), ErrorNo())
#define PerlIO_error(f)			piStdIO->Error((f), ErrorNo())
#define PerlIO_clearerr(f)		piStdIO->Clearerr((f), ErrorNo())
#define PerlIO_getc(f)			piStdIO->Getc((f), ErrorNo())
#define PerlIO_get_base(f)		piStdIO->GetBase((f), ErrorNo())
#define PerlIO_get_bufsiz(f)	piStdIO->GetBufsiz((f), ErrorNo())
#define PerlIO_get_cnt(f)		piStdIO->GetCnt((f), ErrorNo())
#define PerlIO_get_ptr(f)		piStdIO->GetPtr((f), ErrorNo())
#define PerlIO_putc(f,c)		piStdIO->Putc((f),(c), ErrorNo())
#define PerlIO_puts(f,s)		piStdIO->Puts((f),(s), ErrorNo())
#define PerlIO_flush(f)			piStdIO->Flush((f), ErrorNo())
#define PerlIO_ungetc(f,c)		piStdIO->Ungetc((f),(c), ErrorNo())
#define PerlIO_fileno(f)		piStdIO->Fileno((f), ErrorNo())
#define PerlIO_fdopen(f, s)		piStdIO->Fdopen((f),(s), ErrorNo())
#define PerlIO_read(f,buf,count)	(SSize_t)piStdIO->Read((f), (buf), (count), ErrorNo())
#define PerlIO_write(f,buf,count)	piStdIO->Write((f), (buf), (count), ErrorNo())
#define PerlIO_set_cnt(f,c)		piStdIO->SetCnt((f), (c), ErrorNo())
#define PerlIO_set_ptrcnt(f,p,c)	piStdIO->SetPtrCnt((f), (p), (c), ErrorNo())
#define PerlIO_setlinebuf(f)	piStdIO->Setlinebuf((f), ErrorNo())
#define PerlIO_printf			fprintf
#define PerlIO_stdoutf			piStdIO->Printf
#define PerlIO_vprintf(f,fmt,a)		piStdIO->Vprintf((f), ErrorNo(), (fmt),a)          
#define PerlIO_tell(f)			piStdIO->Tell((f), ErrorNo())
#define PerlIO_seek(f,o,w)		piStdIO->Seek((f),(o),(w), ErrorNo())
#define PerlIO_getpos(f,p)		piStdIO->Getpos((f),(p), ErrorNo())
#define PerlIO_setpos(f,p)		piStdIO->Setpos((f),(p), ErrorNo())
#define PerlIO_rewind(f)		piStdIO->Rewind((f), ErrorNo())
#define PerlIO_tmpfile()		piStdIO->Tmpfile(ErrorNo())
#define PerlIO_init()			piStdIO->Init(ErrorNo())
#undef 	init_os_extras
#define init_os_extras()		piStdIO->InitOSExtras(this)

#else
#include "perlsdio.h"
#endif

#ifndef PERLIO_IS_STDIO
#ifdef USE_SFIO
#include "perlsfio.h"
#endif /* USE_SFIO */
#endif /* PERLIO_IS_STDIO */

#ifndef EOF
#define EOF (-1)
#endif

/* This is to catch case with no stdio */
#ifndef BUFSIZ
#define BUFSIZ 1024
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif

#ifndef SEEK_END
#define SEEK_END 2
#endif

#ifndef PerlIO
struct _PerlIO;
#define PerlIO struct _PerlIO
#endif /* No PerlIO */

#ifndef Fpos_t
#define Fpos_t long
#endif

#ifndef NEXT30_NO_ATTRIBUTE
#ifndef HASATTRIBUTE       /* disable GNU-cc attribute checking? */
#ifdef  __attribute__      /* Avoid possible redefinition errors */
#undef  __attribute__
#endif
#define __attribute__(attr)
#endif
#endif

#ifndef PerlIO_stdoutf
extern int	PerlIO_stdoutf		_((const char *,...))
					__attribute__((format (printf, 1, 2)));
#endif
#ifndef PerlIO_puts
extern int	PerlIO_puts		_((PerlIO *,const char *));
#endif
#ifndef PerlIO_open
extern PerlIO *	PerlIO_open		_((const char *,const char *));
#endif
#ifndef PerlIO_close
extern int	PerlIO_close		_((PerlIO *));
#endif
#ifndef PerlIO_eof
extern int	PerlIO_eof		_((PerlIO *));
#endif
#ifndef PerlIO_error
extern int	PerlIO_error		_((PerlIO *));
#endif
#ifndef PerlIO_clearerr
extern void	PerlIO_clearerr		_((PerlIO *));
#endif
#ifndef PerlIO_getc
extern int	PerlIO_getc		_((PerlIO *));
#endif
#ifndef PerlIO_putc
extern int	PerlIO_putc		_((PerlIO *,int));
#endif
#ifndef PerlIO_flush
extern int	PerlIO_flush		_((PerlIO *));
#endif
#ifndef PerlIO_ungetc
extern int	PerlIO_ungetc		_((PerlIO *,int));
#endif
#ifndef PerlIO_fileno
extern int	PerlIO_fileno		_((PerlIO *));
#endif
#ifndef PerlIO_fdopen
extern PerlIO *	PerlIO_fdopen		_((int, const char *));
#endif
#ifndef PerlIO_importFILE
extern PerlIO *	PerlIO_importFILE	_((FILE *,int));
#endif
#ifndef PerlIO_exportFILE
extern FILE *	PerlIO_exportFILE	_((PerlIO *,int));
#endif
#ifndef PerlIO_findFILE
extern FILE *	PerlIO_findFILE		_((PerlIO *));
#endif
#ifndef PerlIO_releaseFILE
extern void	PerlIO_releaseFILE	_((PerlIO *,FILE *));
#endif
#ifndef PerlIO_read
extern SSize_t	PerlIO_read		_((PerlIO *,void *,Size_t));
#endif
#ifndef PerlIO_write
extern SSize_t	PerlIO_write		_((PerlIO *,const void *,Size_t));
#endif
#ifndef PerlIO_setlinebuf
extern void	PerlIO_setlinebuf	_((PerlIO *));
#endif
#ifndef PerlIO_printf
extern int	PerlIO_printf		_((PerlIO *, const char *,...))
					__attribute__((format (printf, 2, 3)));
#endif
#ifndef PerlIO_sprintf
extern int	PerlIO_sprintf		_((char *, int, const char *,...))
					__attribute__((format (printf, 3, 4)));
#endif
#ifndef PerlIO_vprintf
extern int	PerlIO_vprintf		_((PerlIO *, const char *, va_list));
#endif
#ifndef PerlIO_tell
extern long	PerlIO_tell		_((PerlIO *));
#endif
#ifndef PerlIO_seek
extern int	PerlIO_seek		_((PerlIO *,off_t,int));
#endif
#ifndef PerlIO_rewind
extern void	PerlIO_rewind		_((PerlIO *));
#endif
#ifndef PerlIO_has_base
extern int	PerlIO_has_base		_((PerlIO *));
#endif
#ifndef PerlIO_has_cntptr
extern int	PerlIO_has_cntptr	_((PerlIO *));
#endif
#ifndef PerlIO_fast_gets
extern int	PerlIO_fast_gets	_((PerlIO *));
#endif
#ifndef PerlIO_canset_cnt
extern int	PerlIO_canset_cnt	_((PerlIO *));
#endif
#ifndef PerlIO_get_ptr
extern STDCHAR * PerlIO_get_ptr		_((PerlIO *));
#endif
#ifndef PerlIO_get_cnt
extern int	PerlIO_get_cnt		_((PerlIO *));
#endif
#ifndef PerlIO_set_cnt
extern void	PerlIO_set_cnt		_((PerlIO *,int));
#endif
#ifndef PerlIO_set_ptrcnt
extern void	PerlIO_set_ptrcnt	_((PerlIO *,STDCHAR *,int));
#endif
#ifndef PerlIO_get_base
extern STDCHAR * PerlIO_get_base	_((PerlIO *));
#endif
#ifndef PerlIO_get_bufsiz
extern int	PerlIO_get_bufsiz	_((PerlIO *));
#endif
#ifndef PerlIO_tmpfile
extern PerlIO *	PerlIO_tmpfile		_((void));
#endif
#ifndef PerlIO_stdin
extern PerlIO *	PerlIO_stdin	_((void));
#endif
#ifndef PerlIO_stdout
extern PerlIO *	PerlIO_stdout	_((void));
#endif
#ifndef PerlIO_stderr
extern PerlIO *	PerlIO_stderr	_((void));
#endif
#ifndef PerlIO_getpos
extern int	PerlIO_getpos		_((PerlIO *,Fpos_t *));
#endif
#ifndef PerlIO_setpos
extern int	PerlIO_setpos		_((PerlIO *,const Fpos_t *));
#endif
#endif /* Include guard */
