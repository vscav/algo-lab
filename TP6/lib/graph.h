#ifndef GRAPH_H
#define GRAPH_H

struct GraphNode;
#include <queue>
#include <string>
#include <cstring>
#include <stdlib.h>

struct Edge
{
	Edge(GraphNode* source, GraphNode* destination, int distance=0);

	GraphNode* source;
	GraphNode* destination;

	int distance;

	Edge* next;
};

struct GraphNode
{
	GraphNode(int value);
	GraphNode(const GraphNode& other);

	void appendNewEdge(GraphNode* destination, int distance=0);
	std::string toString() const;

	int value;
	Edge* edges;
};

class Graph
{
public:
	Graph(int size=20);

	int nodesCount();
	void appendNewNode(GraphNode* node);

	void buildFromAdjenciesMatrix(int** adjacencies, int nodeCount);

	void deepTravel(GraphNode* first, GraphNode* nodes[], int& nodesSize, bool visited[]);

	void wideTravel(GraphNode* first, GraphNode* nodes[], int& nodesSize, bool visited[]);

	bool detectCycle()
	{
		if (!_nodesCount)
			return false;

		bool* visited = new bool[_nodesCount];
		bool result = false;
		for (int i=0; i<_nodesCount; ++i)
		{
			GraphNode* node = nodes[i];
			memset(visited, 0, sizeof(bool) * _nodesCount);
			if(detectCycle(node, visited))
			{
				result = true;
				break;
			}
		}
		delete visited;
		return result;
	}

	bool detectCycle(GraphNode* first, bool visited[]);


	GraphNode& operator[](int index);

private:
	GraphNode** nodes;
	int allocatedSize;
	int _nodesCount;
};

#endif // GRAPH_H
