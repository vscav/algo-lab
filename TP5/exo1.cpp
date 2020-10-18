#include "lib/tp5.h"
#include <QApplication>
#include <time.h>

MainWindow* w = nullptr;

int HashTable::hash(std::string element)
{
    int hashValue = element[0];
    if (hashValue > (int)size())
        return hashValue % size();
    else
        return hashValue;
}

void HashTable::insert(std::string element)
{
    set(hash(element), element);
}

bool HashTable::contains(std::string element)
{
    int hashValue = hash(element);
    string data = get(hashValue);

    if (data == element)
        return true;
    else
        return false;
}

/**
 * @brief Test function, do not modify
 */
void HashTableInsertThread::assertHashTable(HashTable *table, std::vector<string>& content)
{
    std::vector<string> inserted;
    for (size_t i=0; i<table->size();++i)
    {
        int index = qrand() % (5+i*3);
        index = index %table->size();
        string& data = content[index];
        if (table->get(table->hash(data)) == "")
        {
            table->insert(data);
            inserted.push_back(data);
        }
    }
    for (size_t i=0; i<inserted.size(); ++i)
    {
        string s = inserted[i];
        int index = table->hash(s);
        if (table->get(index) != s)
        {
            QString message("%1 is not at %2, ' %3 ' found instead !");
            throw std::runtime_error(message.arg(QString::fromStdString(s)).arg(index)
                                     .arg(QString::fromStdString(table->get(index))).toStdString());
        }
    }
}

/**
 * @brief Test function, do not modify
 */
void HashTableContainsThread::assertHashTable(HashTable *table, std::vector<std::string> &content)
{
    for (size_t i=0; i<table->size();++i)
    {
        int value = qrand() % 20;
        table->insert(content[value]);
    }

    for (int i=0; i<5; i++)
    {
        int index = qrand() % 20;
        string s = HashTable::names[index];
        bool contains = false;
        for (size_t i=0; i<table->size(); i++)
            if (table->get(i) == s)
            {
                contains = true;
                break;
            }

        if (table->contains(s) != contains)
        {
            QString message;
            if (contains)
                message = "%1 is in the table but you didn't find it";
            else
                message = "%1 is not in the table but you found it";
            throw std::runtime_error(message.arg(QString::fromStdString(s)).toStdString());
        }
    }
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow::instruction_duration = 10;
	w = new HashWindow();
	w->show();

	return a.exec();
}
