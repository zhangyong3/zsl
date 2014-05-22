#ifndef RB_TREE_H
#define RB_TREE_H
#include <string.h>
namespace zsl {
class TreeNode
{
public:
	typedef enum {RED, BLACK} COLOR_T;
	TreeNode(COLOR_T c = RED)
	{
		color = c;
		parent = left = right = NULL;
	}
	virtual ~TreeNode() {}
	virtual int compare(const TreeNode &node) = 0;
	virtual void copy(const TreeNode &node) = 0;
public:
	TreeNode *parent;
	TreeNode *left;
	TreeNode *right;
	COLOR_T color;
};


class RBTree
{
public:
	RBTree();
	virtual ~RBTree();

	TreeNode *find(const TreeNode *z);
	void insert(TreeNode *data);
	TreeNode *remove(TreeNode *data);
	void removeAll();
	size_t size() const {return size_;}

public:
	const TreeNode *getSuccessor(const TreeNode *x) const;
	const TreeNode *getAncestor(const TreeNode *x) const;
protected:
	void initNode(TreeNode *node, TreeNode::COLOR_T color);
	void leftRotate(TreeNode *x);
	void rightRotate(TreeNode *y);
	void insertFixup(TreeNode *z);
	void removeFixup(TreeNode *x);
	void freeNode(TreeNode *z);

public:
	struct NilNode : public TreeNode {
		NilNode() : TreeNode(TreeNode::BLACK) {}
		int compare(const TreeNode &node) { return 0;}
		void copy(const TreeNode &node) {}
	};
	NilNode nil;

protected:
	TreeNode *root;
	size_t size_;
};

}
#endif
