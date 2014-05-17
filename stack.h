#ifndef _ZSL_STACK_H
#define _ZSL_STACK_H

#include "list.h"

namespace zsl {

template<class T, class C=List<T> >
class Stack
{
public:
	Stack(size_t max_size = 0)
		:max(max_size)
	{
	}

	virtual ~Stack()
	{
	}

	bool push(const T &data)
	{
		if (max != 0 && size() >= max) {
			return false;
		}
		items.push_back(data);
		return true;
	}

	void pop()
	{
		items.pop_back();
	}

	T &top()
	{
		return items.back();
	}

	const T &top() const
	{
		return items.back();
	}

	bool empty()
	{
		return items.size() == 0;
	}

	int size()
	{
		return items.size();
	}

protected:
	size_t max;
	C items;
};

}

#endif
