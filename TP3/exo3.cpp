#include "lib/tp3.h"
#include <QApplication>
#include <time.h>
#include <queue>
#include <stack>

#include<QDebug>

MainWindow* w = nullptr;
using std::size_t;

struct BinarySearchTree : public BinaryTree
{
    BinarySearchTree(int value=0) : BinaryTree(value) {}
    virtual ~BinarySearchTree() {}

    void insertNumber(int value)
    {
        Node* iterator = this;
        Node* lastMatch = NULL;

        while (iterator != NULL)
        {
            lastMatch = iterator;
            if (value < iterator->value) iterator = iterator->left;
            else iterator = iterator->right;
        }
        if (lastMatch == NULL) lastMatch = createNode(value);
        else if (value < lastMatch->value) lastMatch->left = createNode(value);
        else lastMatch->right = createNode(value);
    }

    uint height() const
    {
        uint height = 0;
        std::queue<Node*> q;
        Node* root = (Node*)this;
        q.push(root);

        while (true)
        {
            int nodeCount = q.size();
            if (nodeCount == 0) return height;
            height++;

            while (nodeCount > 0)
            {
                Node* node = q.front();
                q.pop();
                if (node->left != NULL) q.push(node->left);
                if (node->right != NULL) q.push(node->right);
                nodeCount--;
            }
        }
    }

    uint nodesCount() const
    {
        uint count = 1;
        std::queue<Node*> q;
        Node* root = (Node*)this;
        q.push(root);

        while (!q.empty())
        {
            Node* temp = q.front();
            q.pop();
            if (temp->left != NULL)
            {
                q.push(temp->left);
                count++;
            }
            if (temp->right != NULL)
            {
                q.push(temp->right);
                count++;
            }
        }
        return count;
    }

    bool isLeaf() const
    {
        if (this->left == NULL && this->right == NULL) return true;
        else return false;
    }

    void allLeaves(Node* leaves[], uint& leavesCount)
    {
        std::queue<Node*> q;
        Node* root = this;
        q.push(root);

        while (!q.empty())
        {
            Node* temp = q.front();
            q.pop();

            if (temp->left != NULL) q.push(temp->left);
            if (temp->right != NULL) q.push(temp->right);
            if (temp->left == NULL && temp->right == NULL)
            {
                leaves[leavesCount] = this;
                leavesCount++;
            }
        }
    }

    // Rule: LPR (Left, Parent, Right)
    void inorderTravel(Node* nodes[], uint& nodesCount)
    {
        if(this->left != NULL) this->left->inorderTravel(nodes, nodesCount);
        nodes[nodesCount] = this;
        nodesCount++;
        if(this->right != NULL) this->right->inorderTravel(nodes, nodesCount);
    }

    // Rule: PLR (Parent, Left, Right)
    void preorderTravel(Node* nodes[], uint& nodesCount)
    {
        nodes[nodesCount] = this;
        nodesCount++;
        if(this->left != NULL) this->left->preorderTravel(nodes, nodesCount);
        if(this->right != NULL) this->right->preorderTravel(nodes, nodesCount);
    }

    // Rule : LRP (Left, Right, Parent)
    void postorderTravel(Node* nodes[], uint& nodesCount)
    {
        if(this->left != NULL) this->left->postorderTravel(nodes, nodesCount);
        if(this->right != NULL) this->right->postorderTravel(nodes, nodesCount);
        nodes[nodesCount] = this;
        nodesCount++;
    }

    Node* find(int value)
    {
        std::queue<Node*> q;
        Node* root = this;
        q.push(root);

        while (!q.empty())
        {
            Node* temp = q.front();
            q.pop();
            if (temp->left != NULL) q.push(temp->left);
            if (temp->right != NULL) q.push(temp->right);
            if (temp->value == value) return temp;
        }
        return nullptr;
    }
};

Node* createNode(int value) {
    return new BinarySearchTree(value);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow::instruction_duration = 200;
    BinarySearchTree bst;
    w = new BinarySearchTreeWindow(bst);
    w->show();

    return a.exec();
}
