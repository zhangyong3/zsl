#include "link.h"
#include <stdio.h>

namespace zsl {
Node::Node()
:prev(NULL), next(NULL)
{
}

Node::~Node()
{
}

Node *Node::insertAfter(Node *p)
{
	if (p != NULL) {
		if (next != NULL) {
			next->prev = p;
			p->next = next;
		} else {
			p->next = NULL;
		}

		next = p;
		p->prev = this;
	}

	return this;
}


Node *Node::insertBefore(Node *p)
{
	if (p != NULL) {
		if (prev != NULL) {
			prev->next = p;
			p->prev = prev;
		}

		prev = p;
		p->next = this;
	}

	return this;
}

Node *Node::remove()
{
	if (prev != NULL) {
		prev->next = next;
	}
	
	if (next != NULL) {
		next->prev = prev;
	}

	prev = next = NULL;
	return this;
}

Node *Node::getNodeSkip(int i)
{
	if (i < 0) return NULL;
	Node *p = this;

	while (p && i-- > 0) {
		p = p->getNext();
	}

	return p;
}

unsigned int Node::distanceTo(Node *n)
{
	Node *p = this;
	unsigned int i = 0;
	while (p) {
		if (p == n) break;
		i++;
		p = p->getNext();
	}
	return i;
}



//class Link
Link::Link()
:count(0), front(NULL), rear(NULL)
{
}

Link::~Link()
{
}


void Link::insertFront(Node *p)
{
	if (front == NULL) {
		front = rear = p;
	} else {
		front->insertBefore(p);
		front = p;
	}
	++count;
}

void Link::insertRear(Node *p)
{
	if (rear == NULL) {
		front = rear = p;
	} else {
		rear->insertAfter(p);
		rear = p;
	}
	++count;
}

Node *Link::removeFront()
{
	return remove(front);
}

Node *Link::removeRear()
{
	return remove(rear);
}

Node *Link::remove(Node *p)
{
	if (p) {
		if (p == front) {
			front = p->getNext();
		}
		if (p == rear) {
			rear = p->getPrev();
		}
		p->remove();
		--count;
	}

	return p;
}

void Link::removeAll()
{
	front = rear = NULL;
	count = 0;
}

}
