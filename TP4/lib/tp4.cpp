#include <QDebug>
#include <QTime>

#include "tp4.h"
#include "lib/array.h"


class HeapChildsThread : public HeapThread
{

public:
    HeapChildsThread(MainWindow* mainWindow, Heap& tree, QObject *parent = nullptr)
        : HeapThread(mainWindow, tree, parent) {}
    void run() override;

    ~HeapChildsThread() override{}
};

class HeapInsertThread : public HeapThread
{

public:
    HeapInsertThread(MainWindow* mainWindow, Heap& tree, QObject *parent = nullptr)
        : HeapThread(mainWindow, tree, parent) {}
    void run() override;

    ~HeapInsertThread() override{}
};

class HeapHeapifyThread : public HeapThread
{

public:
    HeapHeapifyThread(MainWindow* mainWindow, Heap& tree, QObject *parent = nullptr)
        : HeapThread(mainWindow, tree, parent) {}
    void run() override;

    ~HeapHeapifyThread() override{}
};

class HeapSortThread : public HeapThread
{

public:
    HeapSortThread(MainWindow* mainWindow, Heap& tree, QObject *parent = nullptr)
        : HeapThread(mainWindow, tree, parent) {}
    void run() override;

    ~HeapSortThread() override{}
};

HeapWindow::HeapWindow(QWidget *parent) : MainWindow(parent)
{
    heap = &this->newRandomHeap(1);

    threads.push_back(new HeapChildsThread(this, *heap, this));
    threads.push_back(new HeapInsertThread(this, *heap, this));
    threads.push_back(new HeapHeapifyThread(this, *heap, this));
    threads.push_back(new HeapSortThread(this, *heap, this));

    currentThread = threads.begin();
    workerThread = *currentThread;
    connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
}

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
        heapItems.push_back(QVector<HeapNumberGraphicsItem*>());
        HeapNumberGraphicsItem* item = new HeapNumberGraphicsItem(&heap, 0);
        heapItems.last().push_back(item);
        toAdd.append(item);
        dirty = true;
    }

    int i=0;
    for (QVector<HeapNumberGraphicsItem*>& items : heapItems)
    {
        for (HeapNumberGraphicsItem* item : items)
        {
            if (item->needUpdate())
            {
                dirty = true;
                break;
            }
        }
        i++;
    }
    Base::updateScene();
}


void HeapWindow::updateLayout()
{
    int itemWidth = qMax<int>(50, width() * 0.01f);
    int maxX = 0, maxY = 0;
    updateStatusItem(itemWidth);
    updateBackground();
    updateNumberItems(itemWidth, maxX, maxY);
    updateHeapNumberItems(itemWidth, maxX, maxY);
    this->scene.setSceneRect(0, 0, qMax(width(), maxX), qMax(height(),maxY+itemWidth * 2));

    this->dirty=false;
}


void HeapWindow::updateHeapNumberItems(int itemWidth, int &maxX, int &maxY)
{
    for (QVector<HeapNumberGraphicsItem*>& items : heapItems)
    {
        for (HeapNumberGraphicsItem* item : items)
        {
            item->updateNode();
            int treeWidth = item->width();
            int startX = scene.width() * 0.45f;
            item->setRect(startX, maxY, itemWidth, itemWidth);
            item->updateLayout();
            maxY += item->height();
            if (startX + treeWidth > maxX)
            {
                maxX = startX + treeWidth;
            }
        }
    }
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
                QString message("Left Child: Node %1 left child is not node %2 (value=%3) "
								"but node %4 (value=%5)");
				throw std::runtime_error(message.arg(i)
												.arg(this->heap.leftChild(i))
												.arg(this->heap[this->heap.leftChild(i)])
												.arg(left)
												.arg(this->heap[left])
												.toStdString());
			}
			else if (this->heap.rightChild(i) != right) {
                QString message("Right Child: Node %1 right child is not node %2 (value=%3) "
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
    bool update = TreeNumberGraphicsItem::needUpdate() || heap->__get__(i) != _node->value;
    if (update)
        return true;
    if (left && left->needUpdate())
        return true;
    if (right && right->needUpdate())
        return true;
    return false;
}
void HeapInsertThread::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	try {
		qsrand(time(nullptr));


		for (int i=0; i<heap.size();++i)
			heap.__set__(i, -1);

        QTime myTimer;

		for (int i=0; i<heap.size();++i)
		{
            myTimer.start();
            int value = qrand() % 255;
            heap.insertHeapNode(i, value);
		}

		assertHeap();

		_message = QString("Nodes insertion correctly finished !");
		success = true;
	}
	catch(std::exception& e)
	{
        _message = "Insertion: " + QString(e.what());
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
        QTime myTimer;

		for (int i=heap.size() / 2 + 1; i>=0; i--)
		{
            myTimer.start();
            heap.heapify(heap.size(), i);
		}

		assertHeap();

		_message = QString("Nodes heapify correctly finished !");
		success = true;
	}
	catch(std::exception& e)
	{
        _message = "Heapify: " + QString(e.what());
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
                QString message("Sort: Node %1 (value=%2) is greater than Node %3 (value=%4)");
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
    if (heap.effectiveSize() < 2)
    {
        QString message("Insertion: Heap is not correctly filled");
        throw std::runtime_error(message.toStdString());
    }
	for (int i=0; i * 2 + 1<heap.size(); i = i * 2 + 3)
	{
		for (int j=i; j>=0; j = j / 2)
		{
			if (heap.__get__(i * 2 + 1) > heap.__get__(j))
			{
                QString message("Node %1 left child (value=%2) "
                                "is greater than parent %3 (value=%4)");
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
                QString message("Node %1 right child (value=%2) "
                                "is greater than parent %3 (value=%4)");
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
