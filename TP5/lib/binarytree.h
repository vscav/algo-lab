#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <qglobal.h>
#include <QVariant>
#include <vector>

template <typename T>
struct TemplateNode
{
    TemplateNode() {}
    virtual const T* leftChild() const {return nullptr;};
    virtual const T* rightChild() const {return nullptr;};
};

struct Node : TemplateNode<Node>
{
    Node() {}

	virtual ~Node()
	{
		dropChildren();
	};

	virtual uint height() const {return 0;}
	virtual uint nodesCount() const {return 0;}
	virtual bool isLeaf() const {return false;}
	virtual void allLeaves(Node*[], uint&) {return;}
	virtual void inorderTravel(Node*[], uint&) {return;}
	virtual void preorderTravel(Node*[], uint&) {return;}
	virtual void postorderTravel(Node*[], uint&) {return;}
	virtual Node* find(int) {return nullptr;}
    virtual void insertNumber(int) {return;}
    virtual void setValue(QVariant value) {return;}
    virtual QString toString() const {return "";};

	void dropChildren();
    void buildFromValues(const std::vector<int>& values);
};

uint _treeNodesCount(const Node& node);

typedef Node BinaryTree;

#endif // BINARYTREE_H
