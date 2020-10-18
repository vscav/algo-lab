#include "heap.h"

Heap::Heap(uint size)
	: Array(size)
{}

Heap::Heap(const Heap &other)
	: Array(other)
{}

Heap::Heap(const std::vector<int> &other)
	: Array(other)
{}
