#include "binarytree.h"

#include "mainwindow.h"

#include <time.h>
#include <chrono>
#include <thread>

void Node::dropChildren()
{
    if (leftChild())
        delete leftChild();
    if (rightChild())
        delete rightChild();
}

void Node::buildFromValues(const std::vector<int>& values) {
    dropChildren();
    if (values.empty())
        return;
    this->setValue(values[0]);
    for (int i=1; i<values.size(); ++i)
    {
        insertNumber(values[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration));
    }
}

uint _treeNodesCount(const Node &node)
{
    uint count = 1;
    if (node.leftChild())
        count += _treeNodesCount(*node.leftChild());
    if (node.rightChild())
        count += _treeNodesCount(*node.rightChild());
    return count;
}
