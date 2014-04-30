#ifndef _LINK_HPP_
#define _LINK_HPP_

namespace zsl {
class Node
{
public:
	Node();
	virtual ~Node();
	Node *getPrev() {return prev;}
	Node *getNext() {return next;}

	Node *getNodeSkip(int i);
	unsigned int distanceTo(Node *n);

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
	Node *getFront() {return front;}
	Node *getRear() {return rear;}
	const Node *getFront() const {return front;}
	const Node *getRear() const {return rear;}
	int size() const {return count;}

	void insertFront(Node *p);
	void insertRear(Node *p);

	Node *removeFront();
	Node *removeRear();

	Node *remove(Node *p);
	void removeAll();

private:
	int count;
	Node *front;
	Node *rear;
};

}
#endif
