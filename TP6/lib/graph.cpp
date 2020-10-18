#include "graph.h"
#include "mainwindow.h"

#include <stdexcept>
#include <sstream>
#include <memory>
#include <time.h>
#include <chrono>
#include <thread>

#include <QString>

Edge::Edge(GraphNode *source, GraphNode *destination, int distance)
{
	this->next = nullptr;
	this->source = source;
	this->destination = destination;
	this->distance = distance;
}

Graph::Graph(int size)
{
	this->allocatedSize = size;
	this->nodes = new GraphNode*[this->allocatedSize];
	memset(this->nodes, 0, sizeof(GraphNode*) * this->allocatedSize);
	this->_nodesCount = 0;
}

int Graph::nodesCount()
{
	return _nodesCount;
}

void Graph::appendNewNode(GraphNode* node)
{
	this->nodes[_nodesCount] = node;
	this->_nodesCount++;
	if (this->_nodesCount >= this->allocatedSize)
	{
		GraphNode** old = this->nodes;
		this->allocatedSize *= 2;
		this->nodes = new GraphNode*[this->allocatedSize];
		memcpy(this->nodes, old, sizeof(GraphNode*) * this->_nodesCount);
		delete old;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration*2));
}

GraphNode &Graph::operator[](int index)
{
	if (index >= _nodesCount)
	{
		QString error_msg("get(): Index out of bound -> index: %1, size: %2");
		throw std::runtime_error(error_msg.arg(index).arg(_nodesCount)
								 .toStdString());
	}
	return *this->nodes[index];
}

GraphNode::GraphNode(int value)
{
	this->edges = nullptr;
	this->value = value;
}

GraphNode::GraphNode(const GraphNode &other)
{
	this->edges = other.edges;
	this->value = other.value;
}

void GraphNode::appendNewEdge(GraphNode* destination, int distance)
{

	Edge* oldFirst = this->edges;
	this->edges = new Edge(this, destination, distance);
	this->edges->next = oldFirst;
	std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration));
}

std::string GraphNode::toString() const
{
	std::ostringstream stringStream;
	stringStream << " Node<" << this->value << "> ";
	return stringStream.str();
}
