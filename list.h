#ifndef __ZSL_LIST_H
#define __ZSL_LIST_H

#include "link.h"

namespace zsl {


template<class T>
class List
{
protected:
	struct __ListNode : public Node
	{
		__ListNode(const T &d) : data(d) {}
		T data;
	};

	class __List_Iterator
	{
	public:
		__List_Iterator() : cur(NULL), isReverse(false)
		{
		}

		__List_Iterator(__ListNode *p, bool reverse = false)
		 : cur(p), isReverse(reverse)
		{
		}

		T &operator*()
		{
			return cur->data;
		}

		T *operator->()
		{
			return &cur->data;
		}

		__List_Iterator &operator++()
		{
			if (isReverse) {
				cur = (__ListNode *)cur->getPrev();
			} else {
				cur = (__ListNode*)(cur->getNext());
			}
			return *this;
		}

		__List_Iterator operator++(int)
		{
			__ListNode *p = cur;
			if (isReverse) {
				cur = (__ListNode *)cur->getPrev();
			} else {
				cur = (__ListNode*)(cur->getNext());
			}
			return __List_Iterator(p);
		}

		int operator != (const __List_Iterator &it)
		{
			return cur != it.cur;
		}

	public:
		bool isReverse;
		__ListNode *cur;
	};

public:
	typedef __List_Iterator Iterator;
	virtual ~List();

	void clear();

	void push_back(const T &data);
	void push_front(const T &data);

	T pop_front();
	T pop_back();

	T &front();
	T &back();

	size_t size() const
	{
		return list.size();
	}

	Iterator begin()
	{
		return Iterator((__ListNode*)list.getFront());
	}
	Iterator end()
	{
		return Iterator(NULL);
	}

	Iterator rbegin()
	{
		return Iterator((__ListNode*)list.getRear(), true);
	}

	Iterator rend()
	{
		return Iterator(NULL);
	}

	T erase(Iterator it);

	void insert(Iterator it, const T &data)
	{
		__ListNode *p = getNode(data);
		if (p != NULL) {
			list.insertBefore(it.cur, p);
		}
	}
protected:
	__ListNode *getNode(const T &data);

protected:
	Link list;
};

template<class T>
void List<T>::clear()
{
	__ListNode *p;
	while ((p = (__ListNode *)list.removeFront()) != NULL) {
		delete p;
	}
}

template<class T>
List<T>::~List()
{
	clear();
}

template<class T>
void List<T>::push_back(const T &data)
{
	__ListNode *p = getNode(data);
	if (p != NULL) {
		list.insertRear(p);
	}
}

template<class T>
void List<T>::push_front(const T &data)
{
	__ListNode *p = getNode(data);
	if (p != NULL) {
		list.insertFront(p);
	}
}

template<class T>
T List<T>::pop_front()
{
	__ListNode *p = (__ListNode *)list.removeFront();
	return p->data;
}

template<class T>
T List<T>::pop_back()
{
	__ListNode *p = (__ListNode *)list.removeRear();
	return p->data;
}


template<class T>
T List<T>::erase(Iterator it)
{
	__ListNode *p = it.cur;
	list.remove(p);
	it.cur = NULL;

	return p->data;
}


template<class T>
T &List<T>::front()
{
	__ListNode *p = (__ListNode *)list.getFront();
	return p->data;
}

template<class T>
T &List<T>::back()
{
	__ListNode *p = (__ListNode *)list.getRear();
	return p->data;
}

template<class T>
typename List<T>::__ListNode *List<T>::getNode(const T &data)
{
	__ListNode *p = new __ListNode(data);
	return p;
}

}

#endif

