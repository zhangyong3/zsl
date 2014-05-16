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
	virtual int compare(const TreeNode &node) = 0;
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
	size_t Size() const {return size;}

public:
	TreeNode *getSuccessor(TreeNode *x);
	TreeNode *getAncestor(TreeNode *x);
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
	};
	NilNode nil;

protected:
	TreeNode *root;
	size_t size;
};

}
#endif
