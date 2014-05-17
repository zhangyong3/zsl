#ifndef _FILE_H_
#define _FILE_H_

#include "binbuf.h"


namespace zsl {
class File
{
public:
	enum
	{
		APPEND = 1,
		CREATE = (1<<1),
		RDONLY = (1<<2),
		WRONLY = (1<<3),
		RDWR = (1<<4),
		LARGEFILE = (1<<5),
		TRUNC = (1<<6)
	};

	File();
	File(const char *filename, int mode=RDONLY);
	File(const File &f);
	virtual ~File();

	bool open(const char *filename, int mode=RDONLY);
	
	void close();

	const char *getFileName() const { return filename;}

	int read(BinBuf &buf, int len);
	int read(void *buf, int len);

	int write(const void *buf, int len);

	int write(const BinBuf &buf);

	bool readLine(String &s);

	off_t seek(off_t offset, int whence);
protected:
	int fd;
	char *filename;
};
}

#endif
