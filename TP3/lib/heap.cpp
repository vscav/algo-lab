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

HuffmanHeap::HuffmanHeap(uint size)
	: Heap(size)
{
	characters.resize(size);
}

HuffmanHeap::HuffmanHeap(const HuffmanHeap& other)
	: Heap(other),
	  characters(other.characters.begin(),other.characters.end())
{}

char HuffmanHeap::getCharacter(const uint i)
{
	return characters[i];
}

void HuffmanHeap::setCharacter(const uint i, char c)
{
	characters[i] = c;
}

void HuffmanHeap::resize(uint size)
{
	Heap::resize(size);
	characters.resize(size);
}
