#include "string.h"
#include "mempool.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

namespace zsl {
String::String()
:buf(NULL), len(0), capacity(0), pool_(syspool_)
{
}

String::String (MemPool &pool)
:buf(NULL), len(0), capacity(0), pool_(pool)
{
}

String::String (int l)
:buf(NULL), len(0), capacity(0), pool_(syspool_)
{
	resize(l);
}

String::String(const char *s, int l)
:buf(NULL), len(0), capacity(0), pool_(syspool_)
{
	if (l <= 0) {
		if (s != NULL)
			l = strlen(s);
		else
			l = 0;
	}

	if (s && l > 0 && resize(l+1)) {
		memcpy(buf, s, l);
		len = l;
		buf[l] = 0;
	}
}

String::String(const String &str)
:buf(NULL), len(0), capacity(0), pool_(syspool_)
{
	if (str.len > 0 && resize(str.len+1)) {
		memcpy(buf, str.buf, str.len);
		len = str.len;
		buf[len] = 0;
	}
}

String::~String()
{
	if (buf != NULL)
		pool_.free(buf, capacity);
}

bool String::resize(int size)
{
	if (size <= capacity) {
		if (len >= size) {
			len = size-1;
			if (len < 0)
				len = 0;

			if (buf) {
				buf[len] = '\0';
			}
		}
		return true;
	}

	size += 8-size%8;
	char *newbuf = (char*)pool_.reallocate(buf, size, capacity);
	if (!newbuf)
		return false;

	buf = newbuf;
	capacity = size;
	buf[len] = 0;
	return true;
}

const char *String::c_str() const
{
	if (buf)
		return buf;

	return "";
}

int String::length() const
{
	return len;
}

void String::clear()
{
	if (buf) {
		buf[0] = '\0';
		len = 0;
	}
}


String &String::operator = (const char *s)
{
	int l = 0;
	if (s != NULL && (l = strlen(s)) > 0) {
		if (!resize(l+1))
			return *this;

		memcpy(buf, s, l);
		buf[l] = 0;
		len = l;
	} else {
		if (buf != NULL)
			buf[0] = 0;
		len = 0;
	}
	return *this;
}

String &String::operator = (const String &str)
{
	return operator=(str.buf);
}


String &String::operator += (const char *s)
{
	if (s == NULL)
		return *this;

	int l = strlen(s);
	if (l <= 0)
		return *this;

	if (!resize(len+l+1))
		return *this;

	memcpy(buf+len, s, l);
	len += l;
	buf[len] = 0;

	return *this;
}

String &String::operator += (char c)
{
	if (c=='\0') {
		return *this;
	} else {
		char buf[2] = {c, '\0'};
		return operator+=(buf);
	}
}

String &String::operator += (const String &str)
{
	return operator+=(str.c_str());
}

String String::operator + (char c)
{
	String s0(*this);
	s0.operator+=(c);
	return s0;
}

String String::operator + (const char *s)
{
	String s0(*this);
	s0.operator+=(s);

	return s0;
}

String String::operator + (const String &str)
{
	String s0(*this);
	s0.operator+=(str.c_str());

	return s0;
}

bool String::operator !=(const char *s) const
{
	return !operator==(s);
}

bool String::operator !=(const String &str) const
{
	return !operator==(str);
}

bool String::operator == (const char *s) const
{
	if (len != 0 && s != NULL)
		return strcmp(buf, s) == 0;

	if (len == 0 && s == NULL)
		return true;
	else
		return false;
}

bool String::operator == (const String &str) const
{
	if (len != 0 && str.len != 0)
		return strcmp(buf, str.buf) == 0;

	if (len == 0 && str.len == 0)
		return true;
	else
		return false;
}

bool String::operator < (const String &str) const
{
	if (len != 0 && str.len != 0)
		return strcmp(buf, str.buf) < 0;

	if (len == 0 && str.len != 0)
		return true;

	return false;
}

bool String::operator <= (const String &str) const
{
	return operator<(str) || operator==(str);
}

bool String::operator > (const String &str) const
{
	if (len != 0 && str.len != 0)
		return strcmp(buf, str.buf) > 0;

	if (len != 0 && str.len == 0)
		return true;

	return false;
}

bool String::operator >= (const String &str) const
{
	return operator>(str) || operator==(str);
}

String::operator const char *() const
{
	return buf;
}

String &String::trimLeft(const char *s)
{
	if (len <= 0)
		return *this;

	char *p = buf;
	while (strchr(s, *p)) p++;

	if (p != buf) {
		int l = p-buf;
		len-=l;
		memmove(buf, p, len);
		buf[len] = '\0';
	}

	return *this;
}

String &String::trimRight(const char *s)
{
	if (len <= 0)
		return *this;

	char *p = buf+len-1;
	while (p>=buf && strchr(s, *p)) {
		p--;
		len--;
		buf[len] = '\0';
	}

	return *this;
}

String &String::trim(const char *s)
{
	return trimLeft(s).trimRight(s);
}

int String::find(const char *p)
{
	if (buf && p)
		return strstr(buf, p)-buf;

	return npos;
}

int String::find(char c)
{
	if (buf)
		return strchr(buf, c)-buf;

	return npos;
}


int String::rfind(const char *p)
{
	char *last = NULL;
	char *pos = buf;
	while (pos) {
		char *p = strstr(pos, p);
		if (!p) break;

		last = pos;
		pos = p+1;
	}

	if (last)
		return last-buf;

	return npos;
}

int String::rfind(char c)
{
	if (buf)
		return strrchr(buf, c)-buf;

	return npos;
}

String String::substr(int start, int l)
{
	if (len == 0|| start >= l)
		return String("");

	if (len > start+l) {
		return String(buf+start, l);
	} else {
		return String(buf+start, len-start);
	}
}

String String::toLower()  const
{
	String s(len+1);
	for (int i = 0; i < len; ++i)
		s += (char)tolower(buf[i]);

	return s;
}

String String::toUpper()  const
{
	String s(len+1);
	for (int i = 0; i < len; ++i)
		s += (char)toupper(buf[i]);

	return s;
}

int String::toInt(int dft)
{
	if (buf) {
		char *p;
		long r = (long)strtol(buf, &p, 10);
		if (p == buf)
			return dft;

		return (int)r;
	}
	return dft;
}

String String::format(const char *fmt, ...)
{
	char buf[512];
	va_list ap;
	va_start(ap, fmt);
	int n = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	if (n >= sizeof(buf)) {
		try {
			va_list ap2;
			char *p = new char[n+1];
			va_start(ap2, fmt);
			vsprintf(p, fmt, ap2);
			va_end(ap2);

			String s(p);
			delete []p;

			return s;
		} catch (...) {
		}
	} else {
		return String(buf);
	}

	return String("");
}

String operator + (const char *s, const String &str)
{
	String s0(s);
	s0.operator+=(str.c_str());

	return s0;
}


bool String::startsWith(const String &s)
{
	if (len < s.len)
		return false;

	if (len == s.len  && len == 0)
		return true;

	if (buf != NULL && s.buf != NULL) {
		if (memcmp(buf, s.buf, s.len) == 0)
			return true;
	}

	return false;
}

bool String::startsWith(const char *s)
{
	return startsWith(String(s));
}

bool String::endsWith(const String &s)
{
	if (len < s.len)
		return false;

	if (len == s.len  && len == 0)
		return true;

	if (buf != NULL && s.buf != NULL) {
		if (memcmp(buf-s.len, s.buf, s.len) == 0)
			return true;
	}

	return false;
}

bool String::endsWith(const char *s)
{
	return endsWith(String(s));
}

}
