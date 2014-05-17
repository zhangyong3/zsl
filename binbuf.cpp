#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "binbuf.h"

namespace zsl {
BinBuf::BinBuf(int len)
:value_(NULL), len_(0), bufLen_(0), pos_(0)
{
	if (len > 0) {
		value_ = (char*)malloc(len+1);
		bufLen_ = len;
	}
}

BinBuf::BinBuf(const void* buf, int buflen)
:value_(NULL), len_(0), bufLen_(0), pos_(0)
{
	set(0, buf, buflen);
}

BinBuf::BinBuf(const BinBuf &b)
:value_(NULL), len_(0), bufLen_(0)
{
	pos_ = b.pos_;
	set(0, b.value_, b.len_);
}

BinBuf::~BinBuf()
{
	free(value_);
}


void BinBuf::clear()
{
	if (value_ != NULL) {
		value_[0] = 0;
	}
	len_ = 0;
	pos_ = 0;
}

bool BinBuf::resize(int newsize)
{
	if (newsize < bufLen_) {
		if (newsize < len_)
			len_ = newsize;

		if (pos_ >= len_)
			pos_ = len_;
	} else {
		newsize += 8- newsize%8;
		char *newbuf = (char*)realloc(value_, newsize);
		if (!newbuf)
			return false;

		value_ = newbuf;
		bufLen_ = newsize;
	}

	return true;
}

int BinBuf::get(int from, void *buf, int buflen) const
{
	if (from < 0)
		from = 0;

	if (from > len_)
		return 0;

	int l = len_-from;
	if (buflen > l)
		buflen = l;

	if (buflen <= 0)
		return 0;
	memcpy(buf, value_+from, buflen);

	return buflen;
}

int BinBuf::get(int from, int len, BinBuf& buf) const
{
	buf.clear();
	if (!buf.resize(len))
		return 0;

	return get(from, buf.value_, len);
}

int BinBuf::set(int from, const BinBuf&buf)
{
	return set(from, buf.value_, buf.len_);
}

int BinBuf::set(int from, const void *buf, int buflen)
{
	if (buflen <= 0)
		return 0;

	if (buf == NULL)
		return 0;

	if (from < 0)
		from = 0;

	if (!resize(buflen+from+1))
		return 0;

	memcpy(value_+from, buf, buflen);
	int l = from+buflen;
	if (l > len_)
		len_ = l;
	value_[len_] = 0;

	return buflen;
}

int BinBuf::append(const void* buf, int buflen)
{
	if (buf == NULL) {
		return 0;
	}

	if (buflen < 0) {
		buflen = strlen((const char*)buf);
	}

	if (buflen <= 0)
		return 0;

	if (!resize(len_+buflen+1))
		return 0;

	memcpy(value_+len_, buf, buflen);
	len_ += buflen;
	value_[len_] = 0;
	return buflen;
}

int BinBuf::append(const BinBuf &arr)
{
	return append(arr.value_, arr.len_);
}

unsigned char BinBuf::operator[] (int i)
{
	return value_[i];
}

int BinBuf::leftCut(int bytes)
{
	if (len_ < bytes) {
		clear();
		return len_;
	}
	memmove(value_, value_+bytes, len_-bytes);
	len_ -= bytes;
	value_[len_] = 0;
	return bytes;
}

int BinBuf::rightCut(int bytes)
{
	if (len_ < bytes) {
		clear();
		return len_;
	}
	len_ -= bytes;
	value_[len_] = 0;
	return bytes;
}

void *BinBuf::allocBuf(int sz)
{
	clear();
	if (!resize(sz+1))
		return NULL;

	return value_;
}

void BinBuf::releaseBuf(void *buf, int sz)
{
	if (buf != value_)
		return;

	if (sz >= 0) {
		len_ = sz;
		if (pos_ >= len_)
			pos_ = len_;
	}
	if (len_ < bufLen_)
		value_[len_] = 0;
}

int BinBuf::reset(int from)
{
	int f = pos_;
	pos_ = from;
	return f;
}

BinBuf &BinBuf::operator<<(uint8_t d)
{
	pos_ += set(pos_, &d, sizeof(d));
	return *this;
}


BinBuf &BinBuf::operator<<(uint16_t d)
{
	d = htons(d);
	pos_ += set(pos_, &d, sizeof(d));
	return *this;
}

BinBuf &BinBuf::operator<<(uint32_t d)
{
	d = htonl(d);
	pos_ += set(pos_, &d, sizeof(d));
	return *this;
}

BinBuf &BinBuf::operator<<(const char *s)
{
	int sz = strlen(s);
	pos_ += set(pos_, s, sz);
	return *this;
}

BinBuf &BinBuf::operator<<(String &s)
{
	pos_ += set(pos_, s.c_str(), s.length());
	return *this;
}

BinBuf &BinBuf::operator<<(BinBuf &s)
{
	pos_ += set(pos_, s.value_, s.len_);
	return *this;
}


BinBuf &BinBuf::operator>>(uint8_t &d)
{
	d = value_[pos_++];
	return *this;
}

BinBuf &BinBuf::operator>>(uint16_t &d)
{
	d = *(uint16_t*)(value_+pos_);
	pos_+=2;
	d = ntohs(d);
	return *this;
}

BinBuf &BinBuf::operator>>(uint32_t &d)
{
	d = *(uint32_t*)(value_+pos_);
	pos_+=4;
	d = ntohl(d);
	return *this;
}

static const char hex[] = "0123456789abcdef";
String BinBuf::toHexStr() const
{
	return toHexStr(value_, len_);
}

String BinBuf::toHexStrForPrint(size_t align_prepend, size_t size_per_line) const
{
	return toHexStrForPrint(value_, len_, align_prepend, size_per_line);
}

String BinBuf::toHexStr(const void *buf, size_t buf_len)
{
	String s(2*buf_len+1);
	unsigned char *p = (unsigned char*)buf;
	for (int i = 0; i < buf_len; ++i) {
		unsigned char c = p[i];
		s+= hex[c>>4];
		s+= hex[c&0xf];
	}
	return s;
}

String BinBuf::toHexStrForPrint(const void *buf, size_t buf_len, size_t align_prepend, size_t size_per_line)
{
	int n = 0;
	n = (buf_len/size_per_line + 1) *(align_prepend + 3*size_per_line+1+1+ size_per_line) + 1;

	String s(n);
	const uint8_t *p = (const uint8_t*)buf;
	for (size_t i = 0; i < buf_len; i+= size_per_line) {
		size_t j = 0;
		size_t len = size_per_line;
		if (buf_len - i < size_per_line)
			len = buf_len - i;

		j = 0;
		while (j++ < align_prepend) s+="\t";

		j = 0;
		while (j < size_per_line) {
			if (j < len) {
				unsigned char c = p[j];
				s+= hex[c>>4];
				s+= hex[c&0xf];
			} else {
				s+= "  ";
			}
			s+= " ";
			++j;
		}
		s+= "\t";

		j = 0;
		while (j < len) {
			if (isprint(p[j]))
				s+=p[j];
			else
				s+=".";
			++j;
		}

		s+= "\n";
		p+=len;
	}

	return s;
}

int BinBuf::fromHexStr(const char *str, int len)
{
	int i = 0;
	clear();
	resize(len/2+1);
	while (i < len) {
		char c1 = str[i++];
		while (i < len && isspace(c1)) c1=str[i++];

		if (i == len) break;

		if (c1 >= '0' && c1 <= '9')
			c1 = c1-'0';
		else if (c1 >= 'a' && c1 <= 'f')
			c1 = c1-'a'+10;
		else
			break;


		char c2 = str[i++];
		while (i < len && isspace(c2)) c2=str[i++];
		if (i == len) break;

		if (c2 >= '0' && c2 <= '9')
			c2 = c2-'0';
		else if (c2 >= 'a' && c2 <= 'f')
			c2 = c2-'a'+10;
		else
			break;

		char c = c1<<4|c2;
		append(&c, 1);
	}
	return len_;
}

int BinBuf::fromHexStr(const String &s)
{
	return fromHexStr(s.c_str(), s.length());
}
}
