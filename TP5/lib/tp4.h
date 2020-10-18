#ifndef TP4_H
#define TP4_H

#include "mainwindow.h"
#include "heap.h"
#include "thread.h"
#include "TP3.h"

class HeapNumberGraphicsItem : public TreeNumberGraphicsItem
{
public:
	HeapNumberGraphicsItem(Heap* heap, int i, QGraphicsItem* parent=nullptr,
						   float leftShiftting=0, float rightShiftting=0);
	void updateNode();
	bool needUpdate() const;

private:
	Heap* heap;
	int i;
};


class HeapThread : public Thread
{

public:
	HeapThread(MainWindow* mainWindow, Heap& tree, QObject *parent = nullptr)
		: Thread(parent), mainWindow(mainWindow), success(false), heap(tree) {}

	virtual bool succeeded() const {return success;}
	virtual const QString& message() const {return _message;}
	~HeapThread() override{}

	void assertHeap();
protected:
	MainWindow* mainWindow;
	QString _message;
	bool success;

	Heap& heap;
};

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


class HeapWindow : public _TestMainWindow
{
public:
	typedef _TestMainWindow Base;

	HeapWindow(QWidget *parent=nullptr)
		: _TestMainWindow(parent)
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

	Heap& newRandomHeap(uint size);

	void handleResult() override;
	void updateScene();

private:
	QVector<Heap> heaps;
	Heap* heap;
	QLinkedList<Thread*> threads;
	QLinkedList<Thread*>::iterator currentThread;

	QLinkedList<HeapNumberGraphicsItem *> heapItems;
};
#endif // TP4_H
