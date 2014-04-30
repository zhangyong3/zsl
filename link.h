#ifndef _LINK_HPP_
#define _LINK_HPP_

namespace zsl {
class Node
{
public:
	Node();
	virtual ~Node();
	Node *getPrev() {return m_pPrev;}
	Node *getNext() {return m_pNext;}

	Node *getNodeSkip(int i);
	unsigned int distanceTo(Node *n);

protected:
	Node *insertAfter(Node *p);
	Node *insertBefore(Node *p);
	Node *remove();

private:
	Node *m_pPrev;
	Node *m_pNext;

	friend class Link;
};


class Link
{
public:
	Link();
	virtual ~Link();
	Node *getFront() {return m_pFront;}
	Node *getRear() {return m_pRear;}
	const Node *getFront() const {return m_pFront;}
	const Node *getRear() const {return m_pRear;}
	int size() const {return m_nCount;}

	void insertFront(Node *p);
	void insertRear(Node *p);

	Node *removeFront();
	Node *removeRear();

	Node *remove(Node *p);
	void removeAll();

private:
	int m_nCount;
	Node *m_pFront;
	Node *m_pRear;
};

}
#endif
