#ifndef TP5_H
#define TP5_H

#include "array.h"
#include "hashtable.h"
#include "mainwindow.h"
#include "thread.h"

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
	void assertHashTable(std::vector<std::string> &content);

	~HashTableInsertThread() override{}
};

class HashTableContainsThread : public HashTableThread
{

public:
	HashTableContainsThread(MainWindow* mainWindow, HashTable& table, QObject *parent = nullptr)
		: HashTableThread(mainWindow, table, parent) {}
	void run() override;

	~HashTableContainsThread() override{}
};


class HashWindow : public MainWindow
{
public:
	typedef MainWindow Base;

	HashWindow(QWidget *parent=nullptr)
		: MainWindow(parent)
	{
		int n = qrand() % 20 + 5;
		table = &this->newHashTable(n);

		threads.push_back(new HashTableInsertThread(this, *table, this));
		threads.push_back(new HashTableContainsThread(this, *table, this));

		currentThread = threads.begin();
		workerThread = *currentThread;
		connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
	}

	HashTable& newHashTable(int size);

	void handleResult() override;
	void updateScene();

private:
	QVector<HashTable> tables;
	HashTable* table;
	QLinkedList<Thread*> threads;
	QLinkedList<Thread*>::iterator currentThread;
};

#endif // TP5_H
