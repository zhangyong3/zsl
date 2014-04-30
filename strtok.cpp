#include "strtok.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

namespace zsl {
StringTokenizer::StringTokenizer(const char *str, const char *delim)
	:buf(NULL), cur(0), tokens(NULL), token_count(0)
{
	char *p, *q;
	int l = strlen(str);
	if (l <= 0)
		return;

	if (delim == NULL || *delim == '\0')
		return;

	int delim_len = strlen(delim);
	buf = new char[l+1];
	memcpy(buf, str, l);
	buf[l] = '\0';

	p = buf;
	while (*p) {
		q = strstr(p, delim);
		if (q) {
			*q = '\0';
		}

		char **tt = (char**)realloc(tokens, sizeof(char*)*(token_count+1));
		if (!tt)
			break;

		tt[token_count++] = p;
		tokens = tt;

		if (!q)
			break;

		p = q+delim_len;
	}
}

StringTokenizer::StringTokenizer(const char *str)
	:buf(NULL), cur(0), tokens(NULL), token_count(0)
{
	char *p, *q;
	int l = strlen(str);
	if (l <= 0)
		return;

	buf = new char[l+1];
	memcpy(buf, str, l);
	buf[l] = '\0';

	p = buf;
	while (*p) {
		while (*p && isspace(*p)) p++;

		if (*p == '\0')
			break;

		q = p;
		while (*q && !isspace(*q)) q++;

		char **tt = (char**)realloc(tokens, sizeof(char*)*(token_count+1));
		if (!tt)
			break;

		tt[token_count++] = p;
		tokens = tt;

		if (*q == '\0')
			break;

		*q = '\0';
		p = q+1;
	}
}

StringTokenizer::~StringTokenizer()
{
	if (buf)
		delete []buf;

	if (tokens)
		free(tokens);
}

int StringTokenizer::countTokens()
{
	return token_count;
}

bool StringTokenizer::hasMoreTokens()
{
	return cur < token_count;
}

String StringTokenizer::nextToken()
{
	char *p = tokens[cur++];
	return String(p);

}

String StringTokenizer::operator[](int index)
{
	return String(tokens[index]);
}

}
