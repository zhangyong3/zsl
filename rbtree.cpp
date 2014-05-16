#include "rbtree.h"

namespace zsl {

RBTree::RBTree()
	:size(0)
{
	root = &nil;
}

RBTree::~RBTree()
{
	removeAll();
}


TreeNode *RBTree::find(const TreeNode *z)
{
	TreeNode *x = root;
	while (x != &nil) {
		int r = x->compare(*z);
		if (r == 0) {
			return x;
		} else if (r > 0) {
			x = x->left;
		} else {
			x = x->right;
		}
	}

	//return NULL;
	return &nil;
}


void RBTree::insert(TreeNode *z)
{
	TreeNode *x = root;
	TreeNode *y = &nil;
	initNode(z, TreeNode::RED);
	int r = -1;
	while (x != &nil) {
		y = x;
		r = z->compare(*x);
		if (r < 0)
			x = x->left;
		else
			x = x->right;
	}

	z->parent = y;
	if (y == &nil) {
		root = z;
	} else {
		if (r < 0) {
			y->left = z;
		} else {
			y->right = z;
		}
	}

	size++;
	insertFixup(z);
}


TreeNode *RBTree::remove(TreeNode *z)
{
	TreeNode *y;
	TreeNode *x;
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

	TreeNode::COLOR_T color = y->color;
	if (y != z) {
		//z->data = y->data;
		y->parent = z->parent;
		y->left = z->left;
		y->right = z->right;

		if (z->left != &nil) {
			z->left->parent = y;
		}
		if (z->right != &nil) {
			z->right->parent = y;
		}
		if (z->parent != &nil) {
			if (z == z->parent->left) {
				z->parent->left = y;
			} else {
				z->parent->right = y;
			}
		}
		y->color = z->color;
	}

	if (color == TreeNode::BLACK) {
		removeFixup(x);
	}
	size--;

	return z;
}


void RBTree::removeAll()
{
	//while ((volatile TreeNode*)root != &nil) {
	//	delete remove(root);
	//}
}


TreeNode *RBTree::getSuccessor(TreeNode *x)
{
	TreeNode *z = &nil;
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


TreeNode *RBTree::getAncestor(TreeNode *x)
{
	TreeNode *z = &nil;
	if (x->left != &nil) {
		z = x->left;
		while (z->right != &nil)
			z = z->right;
	} else {
		z = x->parent;
		while (z != &nil && x == z->left) {
			x = z;
			z = z->parent;
		}
	}

	return z;
}


void RBTree::freeNode(TreeNode *z)
{
	delete z;
}

void RBTree::initNode(TreeNode *node, TreeNode::COLOR_T color)
{
	node->color = color;
	node->left = node->right = node->parent = &nil;
}


void RBTree::leftRotate(TreeNode *x)
{
	TreeNode *y = x->right;
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


void RBTree::rightRotate(TreeNode *y)
{
	TreeNode *x = y->left;
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


void RBTree::insertFixup(TreeNode *z)
{
	while (z->parent->color == TreeNode::RED) {
		if (z->parent == z->parent->parent->left) {
			TreeNode *y = z->parent->parent->right;
			if (y->color == TreeNode::RED) {
				z->parent->color = TreeNode::BLACK;
				y->color = TreeNode::BLACK;
				z->parent->parent->color = TreeNode::RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->right) {
					z = z->parent;
					leftRotate(z);
				}
				z->parent->color =  TreeNode::BLACK;
				z->parent->parent->color =  TreeNode::RED;
				rightRotate(z->parent->parent);
			}
		} else {
			TreeNode *y = z->parent->parent->left;
			if (y->color == TreeNode::RED) {
				z->parent->color = TreeNode::BLACK;
				y->color = TreeNode::BLACK;
				z->parent->parent->color = TreeNode::RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->left) {
					z = z->parent;
					rightRotate(z);
				}
				z->parent->color =  TreeNode::BLACK;
				z->parent->parent->color =  TreeNode::RED;
				leftRotate(z->parent->parent);
			}
		}
	}
	root->color = TreeNode::BLACK;
}


void RBTree::removeFixup(TreeNode *x)
{
	while (x != root && x->color == TreeNode::BLACK) {
		if (x == x->parent->left) {
			TreeNode *w = x->parent->right;
			if (w->color == TreeNode::RED) {
				w->color = TreeNode::BLACK;
				x->parent->color = TreeNode::RED;
				leftRotate(x->parent);
				w = x->parent->right;
			}

			if (w->left->color == TreeNode::BLACK &&
				w->right->color == TreeNode::BLACK) {
					w->color = TreeNode::RED;
					x = x->parent;
			} else {
				if (w->right->color == TreeNode::BLACK) {
					w->left->color = TreeNode::BLACK;
					w->color = TreeNode::RED;
					rightRotate(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = TreeNode::BLACK;
				w->right->color = TreeNode::BLACK;
				leftRotate(x->parent);
				x = root;
			}
		} else {
			TreeNode *w = x->parent->left;
			if (w->color == TreeNode::RED) {
				w->color = TreeNode::BLACK;
				x->parent->color = TreeNode::RED;
				rightRotate(x->parent);
				w = x->parent->left;
			}

			if (w->right->color == TreeNode::BLACK &&
				w->left->color == TreeNode::BLACK) {
					w->color = TreeNode::RED;
					x = x->parent;
			} else {
				if (w->left->color == TreeNode::BLACK) {
					w->right->color = TreeNode::BLACK;
					w->color = TreeNode::RED;
					leftRotate(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = TreeNode::BLACK;
				w->left->color = TreeNode::BLACK;
				rightRotate(x->parent);
				x = root;
			}
		}
	}
	x->color = TreeNode::BLACK;
}



}
