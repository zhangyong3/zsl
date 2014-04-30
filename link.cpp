#include "link.h"
#include <stdio.h>

namespace zsl {
Node::Node()
:m_pPrev(NULL), m_pNext(NULL)
{
}

Node::~Node()
{
}

Node *Node::insertAfter(Node *p)
{
	if (p != NULL) {
		if (m_pNext != NULL) {
			m_pNext->m_pPrev = p;
			p->m_pNext = m_pNext;
		} else {
			p->m_pNext = NULL;
		}

		m_pNext = p;
		p->m_pPrev = this;
	}

	return this;
}


Node *Node::insertBefore(Node *p)
{
	if (p != NULL) {
		if (m_pPrev != NULL) {
			m_pPrev->m_pNext = p;
			p->m_pPrev = m_pPrev;
		}

		m_pPrev = p;
		p->m_pNext = this;
	}

	return this;
}

Node *Node::remove()
{
	if (m_pPrev != NULL) {
		m_pPrev->m_pNext = m_pNext;
	}
	
	if (m_pNext != NULL) {
		m_pNext->m_pPrev = m_pPrev;
	}

	m_pPrev = m_pNext = NULL;
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
:m_nCount(0), m_pFront(NULL), m_pRear(NULL)
{
}

Link::~Link()
{
}


void Link::insertFront(Node *p)
{
	if (m_pFront == NULL) {
		m_pFront = m_pRear = p;
	} else {
		m_pFront->insertBefore(p);
		m_pFront = p;
	}
	++m_nCount;
}

void Link::insertRear(Node *p)
{
	if (m_pRear == NULL) {
		m_pFront = m_pRear = p;
	} else {
		m_pRear->insertAfter(p);
		m_pRear = p;
	}
	++m_nCount;
}

Node *Link::removeFront()
{
	return remove(m_pFront);
}

Node *Link::removeRear()
{
	return remove(m_pRear);
}

Node *Link::remove(Node *p)
{
	if (p) {
		if (p == m_pFront) {
			m_pFront = p->getNext();
		}
		if (p == m_pRear) {
			m_pRear = p->getPrev();
		}
		p->remove();
		--m_nCount;
	}

	return p;
}

void Link::removeAll()
{
	m_pFront = m_pRear = NULL;
	m_nCount = 0;
}

}
