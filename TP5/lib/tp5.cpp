#include "tp5.h"

HashTable &HashWindow::newHashTable(int size)
{
    tables.push_back(HashTable(size));
    HashTable& array = tables.last();
	this->dirty = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration / 3));
    return array;
}

void HashWindow::updateHashItems(int itemWidth, int &originX, int &originY)
{
    originY += scene.height() * 0.2f;

    for (QVector<NodeGraphicsItem*> items : hashItems)
    {
        int startX = 0.05 * width();
        for (NodeGraphicsItem* item : items)
        {
            item->setRect(startX, originY, itemWidth*1.5, itemWidth* 1.5f);
            startX += itemWidth * 2.f;
            if (startX + itemWidth * 2 > scene.width())
            {
                originY += itemWidth * 2.f;
                startX = qMax((50 + scene.width() - itemWidth * 2.f * items.size()) * 0.5,
                                            0.05 * scene.width());
            }
        }
        if (startX > originX)
            originX = startX;
        originY += itemWidth * 2;
    }
}

void HashTableInsertThread::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	try {
		qsrand(time(nullptr));

        assertHashTable(&table, HashTable::names);

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

void HashTableContainsThread::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	try {
		qsrand(time(nullptr));

        assertHashTable(&table, HashTable::names);

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

void HashWindow::updateScene()
{
    while (hashItems.size() < tables.size())
    {
        HashTable& array = tables[hashItems.size()];
        hashItems.push_back(QVector<NodeGraphicsItem*>());
        for (uint i=0; i < array.size(); ++i)
        {
            NodeGraphicsItem* item = new NodeGraphicsItem(array.get(i));
            hashItems.last().push_back(item);
            toAdd.push_back(item);
        }
    }

    uint i=0;
    for (QVector<NodeGraphicsItem*>& numbers : hashItems)
    {
        HashTable& array = tables[i];
        for (uint j=0; j < array.size(); ++j)
        {
            if ((int)j >= numbers.size())
            {
                numbers.push_back(new NodeGraphicsItem(array.get(j)));
                // toAdd.push_back(numbers[j]);
                dirty = true;
            }
            NodeGraphicsItem* item = numbers[j];
            QVariant data(QString::number(j) + ": " +QString::fromStdString(array.get(j)));
            if (item->data() != data)
            {
                item->setData(data);
                dirty = true;
            }
            item->displayDefault();
        }
        ++i;
    }
    Base::updateScene();
}

void HashWindow::updateLayoutItems(int itemWidth, int& originX, int& originY)
{
    Base::updateLayoutItems(itemWidth, originX, originY);
    updateHashItems(itemWidth, originX, originY);
}

void BinarySearchTreeWindow::addBinaryNode(Node* node)
{
    nodes.push_back(node);
    this->dirty = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration / 3));
}

void BinarySearchTreeWindow::updateLayout()
{
    int itemWidth = qMax<int>(75, width() * 0.01f);
    int maxX = 0, maxY = 0;
    updateStatusItem(itemWidth);
    updateBackground();
    updateNumberItems(itemWidth, maxX, maxY);
    updateTreeItems(itemWidth, maxY, maxY);

    this->scene.setSceneRect(0, 0, qMax(width(), maxX), qMax(height(),maxY+itemWidth * 2));
    this->dirty=false;
}

void BinarySearchTreeWindow::updateTreeItems(int itemWidth, int &maxY, int &maxX)
{

    for (TreeNodeGraphicsItem* item : treeItems)
    {
        item->updateNode();
        int treeWidth = item->width();
        int startX = scene.width() * 0.5f;
        item->setRect(startX, maxY, itemWidth, itemWidth);
        item->updateLayout();
        maxY += item->height();
        if (startX + treeWidth > maxX)
        {
            maxX = startX + treeWidth;
        }
    }
}

void BinarySearchTreeWindow::updateScene()
{
    while (treeItems.size() < nodes.size())
    {
        const Node* node = nodes[treeItems.size()];
        TreeNodeGraphicsItem* item = new TreeNodeGraphicsItem(node);
        treeItems.push_back(item);
        toAdd.append(item);
    }

    for (TreeNodeGraphicsItem* item : treeItems)
    {
        if (item->needUpdate())
            dirty = true;
    }
    Base::updateScene();
}

#ifdef EXO2
void HashTableInsertThread::assertHashTable(HashTable *table, std::vector<string>& content){}
void HashTableContainsThread::assertHashTable(HashTable *table, std::vector<std::string> &content){}
#endif


