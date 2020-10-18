#ifndef TP5_H
#define TP5_H

#include "array.h"
#include "hashtable.h"
#include "mainwindow.h"
#include "thread.h"
#include <QStack>

class HashTableThread : public Thread
{

public:
	HashTableThread(MainWindow* mainWindow, HashTable& table, QObject *parent = nullptr)
		: Thread(parent), mainWindow(mainWindow), success(false), table(table) {}

	virtual bool succeeded() const {return success;}
	virtual const QString& message() const {return _message;}
	~HashTableThread() override{}

protected:
	MainWindow* mainWindow;
	QString _message;
	bool success;

	HashTable& table;
};

class HashTableInsertThread : public HashTableThread
{

public:
	HashTableInsertThread(MainWindow* mainWindow, HashTable& table, QObject *parent = nullptr)
		: HashTableThread(mainWindow, table, parent) {}
	void run() override;
    void assertHashTable(HashTable* table, std::vector<std::string> &content);

	~HashTableInsertThread() override{}
};

class HashTableContainsThread : public HashTableThread
{

public:
	HashTableContainsThread(MainWindow* mainWindow, HashTable& table, QObject *parent = nullptr)
		: HashTableThread(mainWindow, table, parent) {}
	void run() override;
    void assertHashTable(HashTable* table, std::vector<std::string> &content);

	~HashTableContainsThread() override{}
};


class HashWindow : public MainWindow
{
public:
	typedef MainWindow Base;

	HashWindow(QWidget *parent=nullptr)
		: MainWindow(parent)
	{
        int n = qrand() % 20 + 8;
		table = &this->newHashTable(n);

		threads.push_back(new HashTableInsertThread(this, *table, this));
		threads.push_back(new HashTableContainsThread(this, *table, this));

		currentThread = threads.begin();
		workerThread = *currentThread;
		connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
	}

	HashTable& newHashTable(int size);
    void updateHashItems(int itemWidth, int& originX, int& originY);

	void handleResult() override;
    void updateScene() override;
    void updateLayoutItems(int itemWidth, int& originX, int& originY) override;

private:
	QVector<HashTable> tables;
    QVector<QVector<NodeGraphicsItem*> > hashItems;
	HashTable* table;
	QLinkedList<Thread*> threads;
	QLinkedList<Thread*>::iterator currentThread;
};

class BinarySearchTreeWindow : public MainWindow
{
public:
    typedef MainWindow Base;

    BinarySearchTreeWindow(BinaryTree* tree, QWidget *parent=nullptr)
        : MainWindow(parent) {
        QStack<const Node*> nodes;
        nodes.push(tree);
        const Node* left = tree->leftChild();
        while(!nodes.empty())
        {
            const Node* node = nodes.pop();
            if (!node)
                break;
            this->nodes.push_back(node);
            nodes.push(node->leftChild());
            nodes.push(node->rightChild());
        }

        this->dirty = true;
    }
    virtual ~BinarySearchTreeWindow() {}

    void addBinaryNode(Node* node);
    void updateLayout() override;
    void updateScene() override;
    void updateTreeItems(int itemWidth, int &maxY, int &maxX);

private:
    QVector<const Node*> nodes;
    QLinkedList<TreeNodeGraphicsItem *> treeItems;
    QLinkedList<Thread*> threads;
    QLinkedList<Thread*>::iterator currentThread;
};

#endif // TP5_H
