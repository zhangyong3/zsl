#ifndef ARRAY_H
#define ARRAY_H

#include "mempool.h"
#include <new>
#include <string.h>

namespace zsl {
template<class T>
class Vector {
protected:
	struct _VectorIterator
	{
		_VectorIterator(T *items, size_t size, ssize_t index, bool reverse=false)
			: buf_(items), size_(size), index_(index), isReverse(reverse)
		{
		}

		T &operator*()
		{
			return buf_[index_];
		}

		const T &operator*() const
		{
			return buf_[index_];
		}

		T *operator->()
		{
			return &buf_[index_];
		}

		_VectorIterator &operator-(int i)
		{
			if (isReverse) {
				index_+=i;
			} else {
				index_-=i;
			}
			return *this;
		}
		_VectorIterator &operator+(int i)
		{
			if (isReverse) {
				index_-=i;
			} else {
				index_+=i;
			}
			return *this;
		}

		_VectorIterator &operator++()
		{
			if (isReverse) {
				--index_;
			} else {
				++index_;
			}
			return *this;
		}

		_VectorIterator operator++(int)
		{
			int oldIndex = index_;
			if (isReverse) {
				--index_;
			} else {
				++index_;
			}
			return _VectorIterator(buf_, size_, oldIndex, isReverse);
		}

		bool operator == (const _VectorIterator &it)
		{
			return index_ == it.index_ &&
				size_ == it.size_ &&
				buf_ == it.buf_;
		}

		bool operator != (const _VectorIterator &it)
		{
			return !operator==(it);
		}

	public:
		T *buf_;
		size_t size_;
		ssize_t index_;
		bool isReverse;
	};
public:
	typedef T value_type;
	typedef _VectorIterator Iterator;

public:
	Vector();
	Vector(const Vector &v);
	Vector(size_t size, const T &init_value);
	Vector(MemPool &pool);

	virtual ~Vector();

	size_t size() const
	{
		return size_;
	}

	size_t capacity() const
	{
		return cap_;
	}

	void push_back(const T &item)
	{
		insert(size_, item);
	}

	T &back()
	{
		return items_[size_-1];
	}

	const T &back() const
	{
		return items_[size_-1];
	}

	void pop_back()
	{
		if (size_ > 0) {
			size_ -= 1;
			items_[size].~value_type();
		}
	}

	T &front()
	{
		return items_[0];
	}

	const T &front() const
	{
		return items_[0];
	}

	void pop_front()
	{
		if (size_ > 0) {
			items_[0].~value_type();
			size_ -= 1;
			mememove(items_, items_+1, sizeof(T)*(size_));
		}
	}

	void push_front(const T &item)
	{
		insert(0, item);
	}

	T &operator[](int index)
	{
		return items_[index];
	}

	const T &operator[](int index) const
	{
		return items_[index];
	}

	void resize(size_t newsize, const value_type &val = value_type());

	void reserve(size_t size);

	Vector &operator=(const Vector &v);

	Iterator begin()
	{
		return Iterator(items_, size_, 0);
	}

	Iterator end()
	{
		return Iterator(items_, size_, size_);
	}

	Iterator rbegin()
	{
		return Iterator(items_, size_, size_-1, true);
	}

	Iterator rend()
	{
		return Iterator(items_, size_, -1, true);
	}

	void erase(Iterator it)
	{
		if (it.index_ < size_) {
			items_[it.index_].~value_type();
			memmove(items_+it.index_, items_+it.index_+1, sizeof(T)*(size_-it.index_-1));
			--size_;
		}
	}

	void insert(int index, const T &data)
	{
		if (index > size_)
			return;

		if (size_ >= cap_) {
			enlarge(size_);
		}

		if (index < cap_) {
			memmove(items_+index+1, items_+index, sizeof(T)*(size_-index));
			new (items_+index) value_type(data);
			++size_;
		}
	}
protected:
	bool enlarge(size_t size);

protected:
	MemPool &pool_;
	SysMemPool default_pool_;

protected:
	T *items_;
	size_t size_;
	size_t cap_;
};

template<class T>
Vector<T>::Vector() :
	items_(NULL), size_(0), cap_(0), pool_(default_pool_)
{
}

template<class T>
Vector<T>::Vector(const Vector &v) :
	items_(NULL), size_ (v.size_), cap_(0), pool_(v.pool_)
{
	if (size_ > 0 && enlarge(size_)) {
		for (size_t i = 0; i < size_; ++i) {
			new (items_+i) value_type(v[i]);
		}
	}
}

template<class T>
Vector<T>::Vector(MemPool &pool) :
	items_(NULL), size_(0), cap_(0), pool_(pool)
{
}

template<class T>
Vector<T>::Vector(size_t size, const T &init_value) :
	items_(NULL), size_(0), cap_(0), pool_(default_pool_)
{
	enlarge(size);
	for (size_t i = 0; i < size; ++i) {
		new (items_+size_) value_type(init_value);
	}
}


template<class T>
Vector<T>::~Vector()
{
	if (cap_ == 0) {
		return;
	}

	for (size_t i = 0; i < size_; ++i) {
		items_[i].~value_type();
	}
	pool_.free(items_, sizeof(value_type)*(cap_));
}


template<class T>
bool Vector<T>::enlarge(size_t size)
{
	size = size |0x03 +1;
	value_type *new_items_ = (value_type*)pool_.reallocate(
		items_, sizeof(value_type)*(cap_+size), sizeof(value_type)*cap_);

	if (!new_items_) {
		return false;
	}

	cap_+= size;
	items_ = new_items_;
	return true;
}


template<class T>
void Vector<T>::resize(size_t newsize, const value_type &val)
{
	if (newsize > cap_) {
		enlarge(newsize-cap_);
		for (size_t i = size_; i < newsize; ++i) {
			new (items_+i) value_type(val);
		}
	} else {
		for (size_t i = newsize; i < size_; ++i) {
			items_[i].~value_type();
		}
	}
	size_ = newsize;
}

template<class T>
void Vector<T>::reserve(size_t size)
{
	if (size > cap_) {
		enlarge(size - cap_);
	}
}

template<class T>
Vector<T> &Vector<T>::operator=(const Vector<T> &v)
{
	if (this != &v) {
		size_t i;
		if (items_ != NULL) {
			for (i = 0; i < size_; ++i) {
				items_[i].~value_type();
			}
		}

		if (cap_ < v.size_ ) {
			enlarge(v.size_);
		}

		for (i = 0; i < v.size_; ++i) {
			new (items_+i) value_type(v[i]);
		}
		size_ = v.size_; 
	}
	return *this;
}
}
#endif
