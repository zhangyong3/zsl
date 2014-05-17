#ifndef _ZSL_LINK_H_
#define _ZSL_LINK_H_

#include <string.h>

namespace zsl {
class Node
{
public:
	Node();
	virtual ~Node();
	Node *getPrev() const {return prev;}
	Node *getNext() const {return next;}

	const Node *getNodeSkip(int i) const;
	unsigned int distanceTo(Node *n) const;

protected:
	Node *insertAfter(Node *p);
	Node *insertBefore(Node *p);
	Node *remove();

private:
	Node *prev;
	Node *next;

	friend class Link;
};


class Link
{
public:
	Link();
	virtual ~Link();
	//Node *getFront() {return front;}
	//Node *getRear() {return rear;}
	const Node *getFront() const {return front;}
	const Node *getRear() const {return rear;}
	size_t size() const {return count;}

	void insertFront(Node *p);
	void insertRear(Node *p);

	Node *removeFront();
	Node *removeRear();

	Node *remove(Node *p);
	void removeAll();

private:
	size_t count;
	Node *front;
	Node *rear;
};

}
#endif
