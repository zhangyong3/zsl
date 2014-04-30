#ifndef _STR_TOK_H
#define _STR_TOK_H

#include "string.h"

namespace zsl {
	class StringTokenizer
	{
	public:
		StringTokenizer(const char *str);
		StringTokenizer(const char *str, const char *delim);
		~StringTokenizer();

		int countTokens();
		bool hasMoreTokens();
		String nextToken();
		String operator[](int index);

	protected:
		char *buf;
		int cur;
		char **tokens;
		int token_count;
	};
}

#endif
