#include "TP5.h"

HashTable &HashWindow::newHashTable(int size)
{
	HashTable* array = new HashTable(size);
	this->dirty = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration / 3));
	return *array;
}

void HashTableInsertThread::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	try {
		qsrand(time(nullptr));

		std::vector<string> content;
		for (int i=0; i<table.size();++i)
		{
			int value = qrand() % 20;
			table.insert(HashTable::names[value]);
			content.push_back(HashTable::names[value]);
		}

		assertHashTable(content);

		_message = QString("Hash insertion correctly finished !");
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

int hash(HashTable& t, string s)
{
	return ((int)s[0]) % t.size();
}

void HashTableInsertThread::assertHashTable(std::vector<string>& content)
{
	for (int i=0; i<content.size(); ++i)
	{
		string s = content[i];
		int index = hash(table, s);
		if (index != table.hash(s) || table[index] != s)
		{
			QString message("%1 is not at %2, ' %3 ' found instead !");
			throw std::runtime_error(message.arg(QString::fromStdString(s)).arg(index)
									 .arg(QString::fromStdString(table[index])).toStdString());
		}
	}
}

void HashTableContainsThread::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	try {
		qsrand(time(nullptr));

		for (int i=0; i<table.size();++i)
		{
			int value = qrand() % 20;
			table.insert(HashTable::names[value]);
		}

		for (int i=0; i<5; i++)
		{
			int index = qrand() % 20;
			string s = HashTable::names[index];
			bool contains = false;
			for (int i=0; i<table.size(); i++)
				if (table[i] == s)
				{
					contains = true;
					break;
				}

			if (table.contains(s) != contains)
			{
				QString message;
				if (contains)
					message = "%1 is in the table but you didn't find it";
				else
					message = "%1 is not in the table but you found it";
				throw std::runtime_error(message.arg(QString::fromStdString(s)).toStdString());
			}
		}

		_message = QString("Hash contains method works !");
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

void HashWindow::handleResult()
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
