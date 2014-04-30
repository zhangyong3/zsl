#ifndef STRING_H
#define STRING_H

namespace zsl {
	class String
	{
	public:
		String ();
		explicit String (int len);
		String (const char *s, int len=-1);
		String (const String &str);
		virtual ~String();
		static const int npos = -1; 

		void clear();
		bool resize(int size);

		String &operator =(const char *s);
		String &operator =(const String &str);
		String &operator += (char c);
		String &operator += (const char *s);
		String &operator += (const String &str);

		String operator + (char c);
		String operator + (const char *s);
		String operator + (const String &str);

		bool operator ==(const char *s) const;
		bool operator ==(const String &str) const;
		bool operator !=(const char *s) const;
		bool operator !=(const String &str) const;

		bool operator <(const String &str) const;
		bool operator <=(const String &str) const;
		bool operator >(const String &str) const;
		bool operator >=(const String &str) const;

		int length() const;
		const char *c_str() const;
		operator const char *() const;

		String &trimLeft(const char *s=" \t\r\n");
		String &trimRight(const char *s=" \t\r\n");
		String &trim(const char *s=" \t\r\n");

		int find(const char *p);
		int find(char c);
		int rfind(const char *p);
		int rfind(char c);

		String substr(int start, int len);

		String toLower() const;
		String toUpper() const;
		int toInt(int dft = 0);

		bool startsWith(const String &s);
		bool startsWith(const char *s);
		bool endsWith(const String &s);
		bool endsWith(const char *s);

		static String format(const char *fmt, ...);
		operator const char *() {return c_str();}
	protected:
		char *buf;
		int len;
		int capacity;
	};

	String operator + (const char *s, const String &str);


} // end of namespace

#endif
