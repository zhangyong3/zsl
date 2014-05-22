#include "variant.h"
#include <stdlib.h>
#include <stdexcept>

namespace zsl {
const Variant *&Variant::operator [](int index) const
{
	return operator[](index);
}
const Variant *&Variant::operator [](const char *key) const
{
	return operator[](key);
}


IntVariant::IntVariant(int i)
	: value(i)
{
}

IntVariant::IntVariant(const char *str)
	: value(atoi(str))
{
}

bool IntVariant::isNull()  const
{
	return false;
}

int IntVariant::toInt()  const
{
	return value;
}

double IntVariant::toDouble() const
{
	return (double)value;
}

String IntVariant::toString() const
{
	return String::format("%d", value);
}

Variant *&IntVariant::operator [](int index) throw (std::exception)
{
	throw std::invalid_argument("invalid operation [int] on IntVariant");
}

Variant *&IntVariant::operator [](const char *key)
{
	throw std::invalid_argument("invalid operation [const char *] on IntVariant");
}


StringVariant::StringVariant(int i)
{
	value = String::format("%d", i);
}

StringVariant::StringVariant(const char *str)
	: value(str)
{
}

bool StringVariant::isNull()  const
{
	return false;
}

int StringVariant::toInt()  const
{
	return atoi(value.c_str());
}

double StringVariant::toDouble() const
{
	return strtod(value.c_str(), NULL);
}

String StringVariant::toString() const
{
	return value;
}

Variant *&StringVariant::operator [](int index) throw (std::exception)
{
	throw std::invalid_argument("invalid operation [int] on StringVariant");
}

Variant *&StringVariant::operator [](const char *key)
{
	throw std::invalid_argument("invalid operation [const char *] on StringVariant");
}



bool ArrayVariant::isNull()  const
{
	return false;
}

int ArrayVariant::toInt()  const
{
	throw std::invalid_argument("invalid operation toInt on ArrayVariant");
}

double ArrayVariant::toDouble() const
{
	throw std::invalid_argument("invalid operation toDouble on ArrayVariant");
}

String ArrayVariant::toString() const
{
	String str = "[";
	for (int i = 0; i < value.size(); ++i) {
		str += value[i]->toString();
		if (i != value.size()-1) {
			str += ",";
		}
	}

	str += "]";
	return str;
}

Variant *&ArrayVariant::operator [](int index) throw (std::exception)
{
	return value[index];
}

Variant *&ArrayVariant::operator [](const char *key)
{
	return operator[](atoi(key));
}

void ArrayVariant::insert(int index, Variant *data)
{
	value.insert(index, data);
}

void ArrayVariant::push_back(Variant *data)
{
	value.push_back(data);
}




bool DictVariant::isNull()  const
{
	return false;
}

int DictVariant::toInt()  const
{
	throw std::invalid_argument("invalid operation toInt on DictVariant");
}

double DictVariant::toDouble() const
{
	throw std::invalid_argument("invalid operation toDouble on DictVariant");
}

String DictVariant::toString() const
{
	String str = "{";
	for (Map<String, Variant*>::Iterator it = value.cbegin(); it != value.cend() ; ++it) {
		str += it->first;
		str += ":";
		str += it->second->toString();
		str += ",";
	}

	if (str.length() > 1) {
		str.trimRight(",");
	}

	str += "}";
	return str;
}

Variant *&DictVariant::operator [](int index) throw (std::exception)
{
	char buf[16];
	sprintf(buf, "%d", index);
	return value[buf];
}

Variant *&DictVariant::operator [](const char *key)
{
	return value[key];
}
}
