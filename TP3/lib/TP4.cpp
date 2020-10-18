#include "TP4.h"

Heap &HeapWindow::newRandomHeap(uint size)
{
	heaps.push_back(Heap(size));
	Heap& array = heaps.last();
	array.fillSortedRandom(0,500);
	this->dirty = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration / 3));
	return array;
}

void HeapWindow::updateScene()
{
	while (heapItems.size() < heaps.size())
	{
		Heap& heap = heaps[heapItems.size()];
		HeapNumberGraphicsItem* item = new HeapNumberGraphicsItem(&heap, 0);
		heapItems.push_back(item);
		toAdd.append(item);
	}

	for (HeapNumberGraphicsItem* item : heapItems)
	{
		if (item->needUpdate())
			dirty = true;
	}
	Base::updateScene();
}


void HeapChildsThread::run()
{
	try {
		qsrand(time(nullptr));

		int size = qrand() % 10 + 10;
		heap.resize(size);
		heap.fillRandom(0, 200);

		for (int i=0; i < this->heap.size() / 2; ++i)
		{
			int left = i * 2 + 1;
			int right = i * 2 + 2;
			if (this->heap.leftChild(i) != left)
			{
				QString message("Node %1 left child is not node %2 (value=%3) "
								"but node %4 (value=%5)");
				throw std::runtime_error(message.arg(i)
												.arg(this->heap.leftChild(i))
												.arg(this->heap[this->heap.leftChild(i)])
												.arg(left)
												.arg(this->heap[left])
												.toStdString());
			}
			else if (this->heap.rightChild(i) != right) {
				QString message("Node %1 right child is not node %2 (value=%3) "
								"but node %4 (value=%5)");
				throw std::runtime_error(message.arg(i)
												.arg(this->heap.rightChild(i))
												.arg(this->heap[this->heap.rightChild(i)])
												.arg(right)
												.arg(this->heap[right])
												.toStdString());
			}

		}
		_message = QString("Heap is well indexed !");
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

void HeapWindow::handleResult()
{
	Base::handleResult();
	this->dirty = true;
	if (!(*currentThread)->succeeded())
		return;
	currentThread++;
	if (currentThread != threads.end())
	{
		workerThread = *currentThread;
		connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
	}
}

HeapNumberGraphicsItem::HeapNumberGraphicsItem(Heap *heap, int i,
											   QGraphicsItem *parent, float leftShiftting,
											   float rightShiftting)
	: TreeNumberGraphicsItem(new Node(heap->get(i)), parent, leftShiftting, rightShiftting),
	  heap(heap), i(i)
{}

void HeapNumberGraphicsItem::updateNode()
{
	if (!parentItem())
	{
		if (right)
			rightShifting = right->_treeHeight() + 1;

		if (left)
			leftShifting = left->_treeHeight() + 1;
	}
	if (number() != heap->__get__(i))
	{
		_node->value = heap->__get__(i);
		setNumber(_node->value);
	}
	if (i * 2 + 1 < heap->size())
	{
		if (!left)
			left = new HeapNumberGraphicsItem(heap, i * 2 + 1, this);
		left->leftShifting = left->_treeHeight();
		left->rightShifting = (rightShifting / _treeHeight()) / 2 ;
		left->updateNode();
		update();
	}
	if (i * 2 + 2 < heap->size())
	{
		if (!right)
			right = new HeapNumberGraphicsItem(heap, i * 2 + 2, this);
		right->leftShifting = (leftShifting / _treeHeight()) / 2;
		right->rightShifting = right->_treeHeight();
		right->updateNode();
		update();
	}
}

bool HeapNumberGraphicsItem::needUpdate() const
{
	return TreeNumberGraphicsItem::needUpdate() || heap->__get__(i) != _node->value;
}
void HeapInsertThread::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	try {
		qsrand(time(nullptr));


		for (int i=0; i<heap.size();++i)
			heap.__set__(i, -1);

		for (int i=0; i<heap.size();++i)
		{
			int value = qrand() % 255;
			heap.insertHeapNode(i, value);
		}

		assertHeap();

		_message = QString("Nodes insertion correctly finished !");
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

void HeapHeapifyThread::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	try {
		qsrand(time(nullptr));


		for (int i=0; i<heap.size();++i)
		{
			int value = qrand() % 255;
			heap.__set__(i, value);
		}

		for (int i=heap.size() / 2 + 1; i>=0; i--)
		{
			heap.heapify(heap.size(), i);
		}

		assertHeap();

		_message = QString("Nodes heapify correctly finished !");
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

void HeapSortThread::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(4000));

	try {
		heap.heapSort();

		for (int i=0; i<heap.size()-1;++i)
		{
			if (heap[i] > heap[i+1])
			{
				QString message("Node %1 (value=%2) is greater than Node %3 (value=%4)");
				throw std::runtime_error(message.arg(i)
												.arg(heap[i])
												.arg(i+1)
												.arg(heap[i+1])
												.toStdString());
			}
		}

		_message = QString("Array is correctly sorted !");
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

void HeapThread::assertHeap()
{
	for (int i=0; i * 2 + 1<heap.size(); i = i * 2 + 3)
	{
		for (int j=i; j>=0; j = j / 2)
		{
			if (heap.__get__(i * 2 + 1) > heap.__get__(j))
			{
				QString message("Node %1 left child (value=%2) is greater than parent %3 (value=%4)");
				throw std::runtime_error(message.arg(i)
												.arg(heap[i * 2 + 1])
												.arg(j)
												.arg(heap[j])
												.toStdString());
			}
			if (!j)
				break;
		}

		for (int j=i; j>=0; j = j / 2)
		{
			if (heap.__get__(i * 2 + 2) > heap.__get__(j))
			{
				QString message("Node %1 right child (value=%2) is greater than parent %3 (value=%4)");
				throw std::runtime_error(message.arg(i)
												.arg(heap[i * 2 + 2])
												.arg(j)
												.arg(heap[j])
												.toStdString());
			}
			if (!j)
				break;
		}
	}
}
