#ifndef RB_TREE_H
#define RB_TREE_H

namespace zsl {
template<class T>
class TreeNode
{
public:
	enum {RED, BLACK};
	TreeNode()
	{
		parent = left = right = this;
	}

	TreeNode(TreeNode<T> *nil)
	{
		parent = left = right = nil;
	}

public:
	T data;

	TreeNode<T> *parent;
	TreeNode<T> *left;
	TreeNode<T> *right;
	int color;
};


template<class T>
class RBTree
{
public:
	RBTree();
	virtual ~RBTree();

	T *find(const T &data);
	void insert(const T &data);
	void remove(const T &data);
	void removeAll();
	unsigned int size() {return size;}
	void walk(void (*fun)(const T &data));

protected:
	TreeNode<T> *remove(TreeNode<T> *z);

	void leftRotate(TreeNode<T> *x);
	void rightRotate(TreeNode<T> *y);
	void insertFixup(TreeNode<T> *z);
	void removeFixup(TreeNode<T> *x);
	TreeNode<T> *getSuccessor(TreeNode<T> *x);
	TreeNode<T> *getNode(const T &data);
	void freeNode(TreeNode<T> *z);
	void walk_1(TreeNode<T> *x, void (*fun)(const T &data));

private:
	TreeNode<T> *root;
	TreeNode<T> nil;
	TreeNode<T> *frees;
	unsigned int size;
};


template<class T>
RBTree<T>::RBTree()
:size(0)
{
	nil.color = TreeNode<T>::BLACK;
	nil.parent = nil.left = nil.right = &nil;
	root = &nil;
	frees = &nil;
}

template<class T>
RBTree<T>::~RBTree()
{
	removeAll();
	TreeNode<T> *x = frees;
	while (x != &nil) {
		TreeNode<T> *y = x;
		x = x->right;
		delete y;
	}
}

template<class T>
T *RBTree<T>::find(const T &data)
{
	TreeNode<T> *x = root;
	while (x != &nil) {
		if (data == x->data)
			return &(x->data);

		if (data < x->data) {
			x = x->left;
		} else {
			x = x->right;
		}
	}

	return NULL;
}

template<class T>
void RBTree<T>::insert(const T &data)
{
	TreeNode<T> *x = root;
	TreeNode<T> *y = &nil;
	while (x != &nil) {
		y = x;
		if (data < x->data)
			x = x->left;
		else
			x = x->right;
	}

	TreeNode<T> *z = getNode(data);
	if (z == NULL)
		return;

	z->parent = y;
	if (y == &nil) {
		root = z;
	} else {
		if (data < y->data) {
			y->left = z;
		} else {
			y->right = z;
		}
	}

	size++;
	insertFixup(z);
}

template<class T>
void RBTree<T>::remove(const T &data)
{
	TreeNode<T> *z = root;
	while (z != &nil) {
		if (data == z->data)
			break;

		if (data < z->data) {
			z = z->left;
		} else {
			z = z->right;
		}
	}

	if (z != &nil) {
		freeNode(remove(z));
	}
}

template<class T>
TreeNode<T> *RBTree<T>::remove(TreeNode<T> *z)
{
	TreeNode<T> *y;
	TreeNode<T> *x;
	if (z->left == &nil || z->right == &nil)
		y = z;
	else
		y = getSuccessor(z);

	if (y->left != &nil)
		x = y->left;
	else
		x = y->right;

	x->parent = y->parent;
	if (y->parent == &nil) {
		root = x;
	} else {
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	}

	if (y != z) {
		z->data = y->data;
	}

	if (y->color == TreeNode<T>::BLACK) {
		removeFixup(x);
	}
	size--;

	return y;
}

template<class T>
void RBTree<T>::removeAll()
{
	while ((volatile TreeNode<T>*)root != &nil) {
		delete remove(root);
	}
}

template<class T>
TreeNode<T> *RBTree<T>::getSuccessor(TreeNode<T> *x)
{
	TreeNode<T> *z = &nil;
	if (x->right != &nil) {
		z = x->right;
		while (z->left != &nil)
			z = z->left;
	} else {
		z = x->parent;
		while (z != &nil && x == z->right) {
			x = z;
			z = z->parent;
		}
	}

	return z;
}

template<class T>
TreeNode<T> *RBTree<T>::getNode(const T &data)
{
	TreeNode<T> *p = NULL;
	if (frees == &nil) {
		p = new TreeNode<T>(&nil);
		if (p == NULL) {
			return NULL;
		}
	} else {
		p = frees;
		frees = frees->right;
		p->right = &nil;
	}

	p->color = TreeNode<T>::RED;
	p->data = data;
	return p;
}

template<class T>
void RBTree<T>::freeNode(TreeNode<T> *z)
{
	z->parent = z->left = z->right = &nil;
	z->right = frees;
	frees = z;
}

template<class T>
void RBTree<T>::leftRotate(TreeNode<T> *x)
{
	TreeNode<T> *y = x->right;
	x->right = y->left;
	if (y->left != &nil) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == &nil) {
		root = y;
	} else {
		if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
	}
	y->left = x;
	x->parent = y;
}

template<class T>
void RBTree<T>::rightRotate(TreeNode<T> *y)
{
	TreeNode<T> *x = y->left;
	y->left = x->right;
	if (x->right != &nil) {
		x->right->parent = y;
	}
	x->parent = y->parent;
	if (y->parent == &nil) {
		root = x;
	} else {
		if (y == y->parent->right) {
			y->parent->right = x;
		} else {
			y->parent->left = x;
		}
	}
	x->right = y;
	y->parent = x;
}

template<class T>
void RBTree<T>::insertFixup(TreeNode<T> *z)
{
	while (z->parent->color == TreeNode<T>::RED) {
		if (z->parent == z->parent->parent->left) {
			TreeNode<T> *y = z->parent->parent->right;
			if (y->color == TreeNode<T>::RED) {
				z->parent->color = TreeNode<T>::BLACK;
				y->color = TreeNode<T>::BLACK;
				z->parent->parent->color = TreeNode<T>::RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->right) {
					z = z->parent;
					leftRotate(z);
				}
				z->parent->color =  TreeNode<T>::BLACK;
				z->parent->parent->color =  TreeNode<T>::RED;
				rightRotate(z->parent->parent);
			}
		} else {
			TreeNode<T> *y = z->parent->parent->left;
			if (y->color == TreeNode<T>::RED) {
				z->parent->color = TreeNode<T>::BLACK;
				y->color = TreeNode<T>::BLACK;
				z->parent->parent->color = TreeNode<T>::RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->left) {
					z = z->parent;
					rightRotate(z);
				}
				z->parent->color =  TreeNode<T>::BLACK;
				z->parent->parent->color =  TreeNode<T>::RED;
				leftRotate(z->parent->parent);
			}
		}
	}
	root->color = TreeNode<T>::BLACK;
}

template<class T>
void RBTree<T>::removeFixup(TreeNode<T> *x)
{
	while (x != root && x->color == TreeNode<T>::BLACK) {
		if (x == x->parent->left) {
			TreeNode<T> *w = x->parent->right;
			if (w->color == TreeNode<T>::RED) {
				w->color = TreeNode<T>::BLACK;
				x->parent->color = TreeNode<T>::RED;
				leftRotate(x->parent);
				w = x->parent->right;
			}

			if (w->left->color == TreeNode<T>::BLACK &&
				w->right->color == TreeNode<T>::BLACK) {
					w->color = TreeNode<T>::RED;
					x = x->parent;
			} else {
				if (w->right->color == TreeNode<T>::BLACK) {
					w->left->color = TreeNode<T>::BLACK;
					w->color = TreeNode<T>::RED;
					rightRotate(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = TreeNode<T>::BLACK;
				w->right->color = TreeNode<T>::BLACK;
				leftRotate(x->parent);
				x = root;
			}
		} else {
			TreeNode<T> *w = x->parent->left;
			if (w->color == TreeNode<T>::RED) {
				w->color = TreeNode<T>::BLACK;
				x->parent->color = TreeNode<T>::RED;
				rightRotate(x->parent);
				w = x->parent->left;
			}

			if (w->right->color == TreeNode<T>::BLACK &&
				w->left->color == TreeNode<T>::BLACK) {
					w->color = TreeNode<T>::RED;
					x = x->parent;
			} else {
				if (w->left->color == TreeNode<T>::BLACK) {
					w->right->color = TreeNode<T>::BLACK;
					w->color = TreeNode<T>::RED;
					leftRotate(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = TreeNode<T>::BLACK;
				w->left->color = TreeNode<T>::BLACK;
				rightRotate(x->parent);
				x = root;
			}
		}
	}
	x->color = TreeNode<T>::BLACK;
}

template<class T>
void RBTree<T>::walk(void (*fun)(const T &data))
{
	walk_1(root, fun);
}

template<class T>
void RBTree<T>::walk_1(TreeNode<T> *x, void (*fun)(const T &data))
{
	if (x == &nil)
		return;

	if (x->left != &nil)
		walk_1(x->left, fun);
	fun(x->data);
	if (x->right != &nil)
		walk_1(x->right, fun);
}

}
#endif
