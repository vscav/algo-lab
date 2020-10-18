#ifndef HEAP_H
#define HEAP_H

#include "array.h"

class Heap : public Array
{
public:
	Heap(uint size=100);
	Heap(const Heap& other);
	Heap(const std::vector<int>& other);

	int leftChild(int nodeIndex);

	int rightChild(int nodeIndex);

	void insertHeapNode(int heapSize, int value);

	void heapify(int heapSize, int nodeIndex);

	void buildHeap(Array& numbers);

	void heapSort();
};

#endif // HEAP_H
