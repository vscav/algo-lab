#include "lib/tp6.h"
#include <QApplication>
#include <time.h>

#include "iostream"

MainWindow* w = nullptr;

void Graph::buildFromAdjenciesMatrix(int **adjacencies, int nodeCount)
{
    // Display matrix
    int k, l;
    for(k = 0; k < nodeCount; k++)
    {
        for(l = 0; l < nodeCount; l++)
            printf("%d ", adjacencies[k][l]);
        printf("\n");
    }

    for(int k = 0; k < nodeCount; k++)
    {
        GraphNode* source = new GraphNode(k);
        this->appendNewNode(source);
    }

    for (int k=0; k < nodeCount; k++)
    {
        for (int j = 0; j < nodeCount; j++)
        {
            if (adjacencies[k][j] != 0)
                operator[](k).appendNewEdge(&operator[](j), adjacencies[k][j]);
        }
    }
}

void Graph::deepTravel(GraphNode *first, GraphNode *nodes[], int &nodesSize, bool visited[])
{
    if(!visited[first->value])
    {
        visited [first->value] = true;
        nodes[nodesSize] = first;
        nodesSize++;
    }
    for (Edge* edge = first->edges; edge != nullptr; edge = edge->next)
    {
        if (!visited[edge->destination->value])
            deepTravel(edge->destination, nodes, nodesSize, visited);
    }
}

void Graph::wideTravel(GraphNode *first, GraphNode *nodes[], int &nodesSize, bool visited[])
{
    std::queue<GraphNode*> nodeQueue;
    nodeQueue.push(first);
    nodesSize = 0;
    visited[first->value] = true;
    GraphNode* node;
    while(!nodeQueue.empty())
    {
        node = nodeQueue.front();
        nodeQueue.pop();
        std::cout << node->value << std::endl;
        visited[node->value] = true;
        nodes[nodesSize] = node;
        nodesSize++;
        for (Edge* edge = first->edges; edge != nullptr; edge = edge->next)
        {
            if (!visited[edge->destination->value])
                nodeQueue.push(edge->destination);
        }
    }
}

bool Graph::detectCycle(GraphNode *first, bool visited[])
{
    std::queue<GraphNode*> nodeQueue;
    nodeQueue.push(first);
    visited[first->value] = true;
    GraphNode* node;
    while(!nodeQueue.empty())
    {
        node = nodeQueue.front();
        nodeQueue.pop();
        if(visited[node->value])
            return true;
        for (Edge* edge = first->edges; edge != nullptr; edge = edge->next)
        {
            if (!visited[edge->destination->value])
                nodeQueue.push(edge->destination);
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow::instruction_duration = 150;
    w = new GraphWindow();
    w->show();

    return a.exec();
}
