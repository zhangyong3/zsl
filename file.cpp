#include "file.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace zsl {
File::File()
	:fd(-1), filename(NULL)
{
}


File::File(const char *filename, int mode)
	:fd(-1), filename(NULL)
{
	open(filename, mode);
}

File::File(const File &f)
	:fd(-1), filename(NULL)
{
	if (f.fd == -1)
		return;

	fd = dup(f.fd);
	if (fd < 0) {
		fd = -1;
		perror("dup file");
		return;
	}

	filename = strdup(f.filename);
}

File::~File()
{
	close();
}

bool File::open(const char *filename, int mode)
{
	int flag = 0;
	if (mode & File::APPEND)
		flag |= O_APPEND;
	if (mode & File::CREATE)
		flag |= O_CREAT;
#ifdef O_LARGEFILE
	if (mode & File::LARGEFILE)
		flag |= O_LARGEFILE;
#endif
	if (mode & File::TRUNC)
		flag |= O_TRUNC;

	if (mode & File::RDWR)
		flag |= O_RDWR;
	else if (mode & File::WRONLY)
		flag |= O_WRONLY;
	else if (mode & File::RDONLY)
		flag |= O_RDONLY;

	close();

	int f = -1;
	if (mode & File::RDONLY)
		f = ::open(filename, flag);
	else
		f = ::open(filename, flag, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);

	if (f < 0)
		return false;

	fd = f;
	filename = strdup(filename);

	return true;
}

void File::close()
{
	if (fd != -1)
		::close(fd);
	fd = -1;

	free(filename);
	filename = NULL;
}

int File::read(BinBuf &buf, int len)
{
	char *x = (char*)buf.allocBuf(len);
	if (!x) return 0;

	int n = read(x, len);
	buf.releaseBuf(x, n);
	return n;
}

int File::read(void *buf, int len)
{
	int n = 0;
	while (n < len) {
		int r = ::read(fd, buf, len-n);			
		if (r < 0) {
			if (errno == EINTR) continue;
			perror("File::Read");
			return r;
		}

		if (r == 0)
			break;

		n += r;
	}
	return n;
}

int File::write(const void *buf, int len)
{
	int n = 0;
	const char *p = (const char *)buf;
	while (n < len) {
		int r = ::write(fd, p+n, len-n);
		if (r < 0) {
			if (errno == EINTR) continue;
			perror("File::Write");
			return r;
		}

		if (r == 0)
			break;

		n += r;
	}
	return n;
}

int File::write(const BinBuf &buf)
{
	return write(buf.getPtr(), buf.size());
}

bool File::readLine(String &s)
{
	char buf[1024];
	buf[sizeof(buf)-1] = 0;

	s.clear();
	while (true) {
		int n = ::read(fd, buf, sizeof(buf)-1);
		if (n < 0) {
			if (errno == EINTR) continue;
			perror("File::ReadLine");
			return false;
		}

		if (n == 0)
			return false;

		char *p = (char*)strchr(buf, '\n');
		if (p) {
			*p = '\0';
			s+=buf;
			int offset = buf+n-p-1;
			if (offset > 0)
				lseek(fd, -offset, SEEK_CUR);
			break;
		}

		s+=buf;
	}

	return true;
}

off_t File::seek(off_t offset, int whence)
{
	return lseek(fd, offset, whence);
}
}

