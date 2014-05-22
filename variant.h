#ifndef VARIANT_H
#define VARIANT_H

#include "string.h"
#include "vector.h"
#include "map.h"
#include "mempool.h"
#include <exception>

namespace zsl {
class Variant
{
public:
	virtual bool isNull() const = 0;
	virtual int toInt() const = 0;
	virtual double toDouble() const = 0;
	virtual String toString() const = 0;
	virtual Variant *&operator [](int index) throw(std::exception) = 0;
	virtual Variant *&operator [](const char *key) = 0;
	virtual const Variant *&operator [](int index) const;
	virtual const Variant *&operator [](const char *key) const;
};

class IntVariant : public Variant
{
public:
	IntVariant(int i);
	IntVariant(const char *str);
	bool isNull() const;
	int toInt() const;
	double toDouble() const;
	String toString() const;
	Variant *&operator [](int index) throw(std::exception);
	Variant *&operator [](const char *key) ;
	int &data() {return value; }

protected:
	int value;
};

class StringVariant : public Variant
{
public:
	StringVariant(int i);
	StringVariant(const char *str);
	bool isNull() const;
	int toInt() const;
	double toDouble() const;
	String toString() const;
	Variant *&operator [](int index) throw(std::exception);
	Variant *&operator [](const char *key) ;
	String &data() {return value; }

protected:
	String value;
};



class ArrayVariant : public Variant
{
public:
	bool isNull() const;
	int toInt() const;
	double toDouble() const;
	String toString() const;
	Variant *&operator [](int index) throw(std::exception);
	Variant *&operator [](const char *key) ;
	Vector<Variant*> &data() {return value; }

	void insert(int index, Variant *);
	void push_back(Variant *);
protected:
	Vector<Variant*> value;
};


class DictVariant : public Variant
{
public:
	bool isNull() const;
	int toInt() const;
	double toDouble() const;
	String toString() const;
	Variant *&operator [](int index) throw(std::exception);
	Variant *&operator [](const char *key) ;
	Map<String, Variant*> &data() {return value; }
protected:
	Map<String, Variant*> value;
};


class VarFactory
{
public:
	VarFactory() : pool_(syspool_) {}
	VarFactory(MemPool &pool) :pool_(pool) {}

	Variant *create(int arg);
	Variant *create(const char* arg);

	ArrayVariant *createArray();
	DictVariant *createDict();
protected:
	MemPool &pool_;
	SysMemPool syspool_;
};

inline ArrayVariant *VarFactory::createArray()
{
	void *p = pool_.allocate(sizeof(ArrayVariant));
	new (p) ArrayVariant();
	return (ArrayVariant*) p;
}

inline DictVariant *VarFactory::createDict()
{
	void *p = pool_.allocate(sizeof(DictVariant));
	new (p) DictVariant();
	return (DictVariant*)p;
}

inline Variant *VarFactory::create(const int arg)
{
	void *p = pool_.allocate(sizeof(IntVariant));
	new (p) IntVariant(arg);
	return (Variant*) p;
}

inline Variant *VarFactory::create(const char* arg)
{
	void *p = pool_.allocate(sizeof(StringVariant));
	new (p) StringVariant(arg);
	return (Variant*) p;
}

} // end of namespace

#endif
