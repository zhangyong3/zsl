#ifndef ARRAY_H
#define ARRAY_H

#include "mempool.h"
#include <new>

namespace zsl {

	template<class T>
	class vector {
	public:
		typedef T value_type;
		vector();
		vector(const vector &v);
		vector(size_t size, const T &init_value);
		vector(mempool &pool);

		virtual ~vector();

		size_t size() const
		{
			return size_;
		}

		size_t capacity() const
		{
			return cap_;
		}

		void push_back(const T &item);

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

		vector &operator=(const vector &v);
	protected:
		bool enlarge(size_t size);

	protected:
		mempool &pool_;
		sysmempool default_pool_;

	protected:
		T *items_;
		size_t size_;
		size_t cap_;
	};

	template<class T>
	vector<T>::vector() :
		items_(NULL), size_(0), cap_(0), pool_(default_pool_)
	{
	}

	template<class T>
	vector<T>::vector(const vector &v) :
		items_(NULL), size_ (v.size_), cap_(0), pool_(v.pool_)
	{
		if (size_ > 0 && enlarge(size_)) {
			for (size_t i = 0; i < size_; ++i) {
				new (items_+i) value_type(v[i]);
			}
		}
	}

	template<class T>
	vector<T>::vector(mempool &pool) :
		items_(NULL), size_(0), cap_(0), pool_(pool)
	{
	}

	template<class T>
	vector<T>::vector(size_t size, const T &init_value) :
		items_(NULL), size_(0), cap_(0), pool_(default_pool_)
	{
		enlarge(size);
		for (size_t i = 0; i < size; ++i) {
			new (items_+size_) value_type(init_value);
		}
	}


	template<class T>
	vector<T>::~vector()
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
	bool vector<T>::enlarge(size_t size)
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
	void vector<T>::push_back(const T &data)
	{
		if (size_ < cap_ || enlarge(1)) {
			new(items_+size_++) value_type (data);
		}
	}


	template<class T>
	void vector<T>::resize(size_t newsize, const value_type &val)
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
	void vector<T>::reserve(size_t size)
	{
		if (size > cap_) {
			enlarge(size - cap_);
		}
	}

	template<class T>
	vector<T> &vector<T>::operator=(const vector<T> &v)
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
