#ifndef _ZSL_SET_H
#define _ZSL_SET_H

#include "rbtree.h"
#include <functional>
#include <utility>

namespace zsl {

template<class T, class C=std::less<T> >
class Set : protected RBTree
{
protected:
	static C comp;
	struct _SetNode : public TreeNode
	{
		T data;
		_SetNode(const T &d) : data(d) {}

		virtual int compare(const TreeNode &node)
		{
			const T &d = ((const _SetNode&)node).data;
			if (comp(data, d)) {
				return -1;
			} else if (comp(d, data)) {
				return 1;
			} else {
				return 0;
			}
		}
		virtual void copy(const TreeNode &node)
		{
			const T &d = ((const _SetNode&)node).data;
			data = d;
		}
	};
	class _SetIterator
	{
	public:
		_SetIterator(RBTree *tree, TreeNode *node, bool reverse=false)
			: tree(tree), cur(node), isReverse(reverse)
		{
		}

		T &operator*()
		{
			return ((_SetNode*)cur)->data;
		}

		T *operator->()
		{
			return &(((_SetNode*)cur)->data);
		}

		_SetIterator &operator++()
		{
			cur = isReverse ? tree->getAncestor(cur) : tree->getSuccessor(cur);
			return *this;
		}

		_SetIterator operator++(int)
		{
			TreeNode *p = cur;
			operator++();
			return _SetIterator(tree, p, isReverse);
		}

		int operator == (const _SetIterator &it)
		{
			return tree == it.tree && cur == it.cur;
		}
		int operator != (const _SetIterator &it)
		{
			return !operator==(it);
		}
		
	public:
		RBTree *tree;
		TreeNode *cur;
		bool isReverse;
	};

public:
	typedef _SetIterator Iterator;

	Iterator begin()
	{
		TreeNode *node = root;
		while (node != &nil && node->left != &nil) {
			node = node->left;
		}
		return Iterator(this, node);
	}
	Iterator end()
	{
		return Iterator(this, &nil);
	}
	Iterator rbegin()
	{
		TreeNode *node = root;
		while (node != &nil && node->right != &nil) {
			node = node->right;
		}
		return Iterator(this, node, true);
	}
	Iterator rend()
	{
		return Iterator(this, &nil, true);
	}

	Iterator find(const T &data)
	{
		_SetNode node(data);
		TreeNode *p = RBTree::find(&node);
		if (p) {
			return Iterator(this, p);
		}
		return end();
	}

	void insert(const T &data)
	{
		_SetNode *node = new _SetNode(data);
		RBTree::insert(node);
	}

	void erase(Iterator it)
	{
		if (it.cur != &nil) {
			delete RBTree::remove(it.cur);
		}
	}

	size_t size() const
	{
		return RBTree::size();
	}
};

template<class T, class C >
C Set<T, C>::comp;
}

#endif
