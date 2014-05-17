#ifndef __BINBUF_H_
#define __BINBUF_H_

#include "string.h"

namespace zsl {
class BinBuf
{
public:
	BinBuf(int len=0);
	BinBuf(const void *buf, int len=-1);
	BinBuf(const BinBuf &b);
	virtual ~BinBuf();

public:
	void clear();
	bool resize(int newsize);

	int size() const {return len_;}

	void *getPtr() { return value_;}
	const void *getPtr() const { return value_;}

	int get(int from, int len, BinBuf& buf) const;
	int get(int from, void *buf, int buflen) const;
	int set(int from, const BinBuf& buf);
	int set(int from, const void *buf, int buflen);

	int append(const void* buf, int buflen=-1);
	int append(const BinBuf &arr);

	int leftCut(int bytes=1);
	int rightCut(int bytes=1);

	void *allocBuf(int sz);
	void releaseBuf(void *buf, int sz);

	unsigned char operator[] (int i);

	int reset(int pos=0);

	BinBuf &operator<<(uint8_t d);
	BinBuf &operator<<(uint16_t d);
	BinBuf &operator<<(uint32_t d);
	BinBuf &operator<<(const char *s);
	BinBuf &operator<<(String &s);
	BinBuf &operator<<(BinBuf &s);

	BinBuf &operator>>(uint8_t &d);
	BinBuf &operator>>(uint16_t &d);
	BinBuf &operator>>(uint32_t &d);

	String toHexStr() const;
	String toHexStrForPrint(size_t align_prepend=0, size_t size_per_line=20) const;
	static String toHexStr(const void *buf, size_t buf_len);
	static String toHexStrForPrint(const void *buf, size_t buf_len, size_t align_prepend=0, size_t size_per_line=20);

	int fromHexStr(const char *str, int len);
	int fromHexStr(const String &s);

protected:
	char *value_;
	int len_;
	int bufLen_;

	int pos_;
};
}

#endif
