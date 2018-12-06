#ifndef __FILE__H__
#define __FILE__H__

#include <stdio.h>
#include <sys/stat.h>

#ifdef HAVE_LIBZ
#include <zlib.h>
#endif

typedef enum
{
	FIO_OPEN,
	FIO_CLOSE,
	FIO_WRITE,
	FIO_RENAME,
	FIO_UNLINK,
	FIO_MKDIR,
	FIO_CHMOD,
	FIO_SEEK,
	FIO_TRUNCATE,
	FIO_READ,
	FIO_LOAD,
	FIO_STAT,
	FIO_FSTAT,
	FIO_SEND,
	FIO_ACCESS,
	FIO_TRANSFER
} fio_operations;

typedef enum
{
	FIO_LOCAL_HOST,
	FIO_DB_HOST,
	FIO_BACKUP_HOST,
	FIO_REMOTE_HOST
} fio_location;

typedef enum
{
	FIO_BACKUP_START_TIME
} fio_shared_variable;

#define FIO_FDMAX 64
#define FIO_PIPE_MARKER 0x40000000

#define SYS_CHECK(cmd) do if ((cmd) < 0) { perror(#cmd); exit(EXIT_FAILURE); } while (0)
#define IO_CHECK(cmd, size) do { int _rc = (cmd); if (_rc != (size)) { fprintf(stderr, "%s:%d: proceeds %d bytes instead of %d\n", __FILE__, __LINE__, _rc, (int)(size)); exit(EXIT_FAILURE); } } while (0)

typedef struct
{
	unsigned cop    : 4;
	unsigned handle : 8;
	unsigned size   : 20;
	unsigned arg;
} fio_header;


typedef struct
{
	void*  address;
	size_t size;
} fio_binding;

extern void    fio_redirect(int in, int out);
extern void    fio_communicate(int in, int out);

extern FILE*   fio_fopen(char const* name, char const* mode, fio_location location);
extern size_t  fio_fwrite(FILE* f, void const* buf, size_t size);
extern size_t  fio_fread(FILE* f, void* buf, size_t size);
extern int     fio_fprintf(FILE* f, char const* arg, ...) __attribute__((format(printf, 2, 3)));
extern int     fio_fflush(FILE* f);
extern int     fio_fseek(FILE* f, off_t offs);
extern int     fio_ftruncate(FILE* f, off_t size);
extern int     fio_fclose(FILE* f);
extern int     fio_ffstat(FILE* f, struct stat* st);

extern int     fio_open(char const* name, int mode, fio_location location);
extern ssize_t fio_write(int fd, void const* buf, size_t size);
extern ssize_t fio_read(int fd, void* buf, size_t size);
extern int     fio_flush(int fd);
extern int     fio_seek(int fd, off_t offs);
extern int     fio_fstat(int fd, struct stat* st);
extern int     fio_truncate(int fd, off_t size);
extern int     fio_close(int fd);

extern int     fio_rename(char const* old_path, char const* new_path, fio_location location);
extern int     fio_unlink(char const* path, fio_location location);
extern int     fio_mkdir(char const* path, int mode, fio_location location);
extern int     fio_chmod(char const* path, int mode, fio_location location);
extern int     fio_access(char const* path, int mode, fio_location location);
extern int     fio_stat(char const* path, struct stat* st, bool follow_symlinks, fio_location location);

extern FILE*   fio_open_stream(char const* name, fio_location location);
extern int     fio_close_stream(FILE* f);

#ifdef HAVE_LIBZ
extern gzFile  fio_gzopen(char const* path, char const* mode, int* tmp_fd, fio_location location);
extern int     fio_gzclose(gzFile file, char const* path, int tmp_fd);
#endif

extern void    fio_transfer(fio_shared_variable var);

#endif
