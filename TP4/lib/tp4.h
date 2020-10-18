#ifndef TP4_H
#define TP4_H

#include "mainwindow.h"
#include "heap.h"
#include "thread.h"
#include "tp3.h"

class HeapNumberGraphicsItem : public TreeNumberGraphicsItem
{
public:
	HeapNumberGraphicsItem(Heap* heap, int i, QGraphicsItem* parent=nullptr,
						   float leftShiftting=0, float rightShiftting=0);
    void updateNode() override;
    bool needUpdate() const override;

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


class HeapWindow : public MainWindow
{
public:
    typedef MainWindow Base;

    HeapWindow(QWidget *parent=nullptr);

	Heap& newRandomHeap(uint size);

	void handleResult() override;
    void updateScene() override;
    void updateLayout() override;
    void updateHeapNumberItems(int itemWidth, int &maxY, int &maxX);

private:
	QVector<Heap> heaps;
	Heap* heap;
	QLinkedList<Thread*> threads;
	QLinkedList<Thread*>::iterator currentThread;

    QVector<QVector<HeapNumberGraphicsItem*> > heapItems;
};

#endif // TP4_H
