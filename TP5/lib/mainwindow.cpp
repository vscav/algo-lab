#include "mainwindow.h"

#include <QDebug>
#include <time.h>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <math.h>
#include <QApplication>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsWidget>
#include <QPair>


unsigned long MainWindow::instruction_duration = 1000;
MainWindow* MainWindow::_instance = nullptr;

MainWindow* MainWindow::instance()
{
    return MainWindow::_instance;
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), timer(this), background(nullptr), zoom(false), scale(1),
	workerThread(nullptr)
{
	initialize();
	resize(1280, 720);

    timer.setInterval(MainWindow::instruction_duration * 0.5);
    MainWindow::_instance = this;
}

void MainWindow::initialize()
{
	qsrand(time(nullptr));

	setBackground(nullptr);

	view.setScene(&scene);
	view.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	view.setMouseTracking(true);
	view.installEventFilter(this);
	view.viewport()->installEventFilter(this);
	this->setCentralWidget(&view);

	toAdd.append(&statusItem);
	statusItem.setDefaultTextColor(QColor(215,235,255));

	connect(&timer, SIGNAL(timeout()), this, SLOT(updateScene()));
	arrays.reserve(40);
	this->resize(600, 600);
    timer.start();
}

Array &MainWindow::newRandomArray(uint size)
{
	arrays.push_back(Array(size));
	Array& array = arrays.last();
	array.fillRandom(0,255);
	this->dirty = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration / 3));
	return array;
}

Array &MainWindow::newSortedRandomArray(uint size)
{
	arrays.push_back(Array(size));
	Array& array = arrays.last();
	array.fillSortedRandom(0,500);
	std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration / 2));
	return array;
}

Array &MainWindow::newArray(uint size)
{
	arrays.push_back(Array(size));
	Array& array = arrays.last();
	std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration / 2));
	return array;
}

void MainWindow::updateLayout()
{

	if (dirty)
	{
		int itemWidth = qMax<int>(50, width() * 0.01f);
		int maxX=0, maxY=0;
        updateLayoutItems(itemWidth, maxX, maxY);
		this->scene.setSceneRect(0, 0, qMax(width(), maxX), qMax(height(),maxY+itemWidth * 2));
	}

    this->dirty=false;
}

void MainWindow::updateLayoutItems(int itemWidth, int& originX, int& originY)
{
    updateStatusItem(itemWidth);
    updateBackground();
    updateNumberItems(itemWidth, originX, originY);
    updateFunctionCalls(itemWidth, originX, originY);
    updateTreeItems(itemWidth, originX, originY);
}


void MainWindow::updateTreeItems(int itemWidth, int &maxX, int &maxY)
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


void MainWindow::updateBackground()
{
	if (background)
	{
		background->setPos(0,0);
		background->setPixmap(background->pixmap().scaledToWidth(scene.width()));
	}
}

int MainWindow::updateNumberItems(int itemWidth, int& maxY, int& maxX)
{
	maxY = scene.height() * 0.2f;
	maxX = 0;
    for (QVector<NodeGraphicsItem*> items : numberItems)
	{
		int startX = 0.05 * width();
        for (NodeGraphicsItem* item : items)
		{
			item->setRect(startX, maxY, itemWidth, itemWidth);
			startX += itemWidth * 1.5f;
			if (startX + itemWidth * 2 > scene.width())
			{
				maxY += itemWidth * 1.5f;
				startX = qMax((50 + scene.width() - itemWidth * 1.5f * items.size()) * 0.5,
											0.05 * scene.width());
			}
		}
		if (startX > maxX)
			maxX = startX;
		maxY += itemWidth * 2;
	}
	return maxY;
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::Wheel)
	{
		QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
		if (QApplication::keyboardModifiers() == Qt::KeyboardModifier::ControlModifier) {
			float wheelDelta = wheel_event->delta();
            scale += pow(1.01f, abs(wheelDelta) / 24.f) * (signbit(wheelDelta) ? -1 : 1) / 5.f;
			scale = qMin<float>(2.5f, qMax<float>(0.25f, scale));
			zoom = true;
			targetPos = wheel_event->pos();
			return true;
		}
	}
    return QMainWindow::eventFilter(object, event);
}

void MainWindow::showEvent(QShowEvent *event)
{
    Base::showEvent(event);
    dirty=true;
    updateLayout();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
	this->dirty = true;
    updateLayout();
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	if (workerThread && workerThread->isRunning())
	{
		MainWindow::instruction_duration = 1;
		workerThread->wait(500);
	}
	QMainWindow::closeEvent(e);
}

void MainWindow::setBackground(QImage *image)
{
	if (background)
	{
		scene.removeItem(background);
	}
	if (!image)
	{
		background = nullptr;
		QRadialGradient gradient(-20, -40, 1920);
		gradient.setSpread(QGradient::ReflectSpread);
		gradient.setColorAt(0,QColor(60,28,52));
		gradient.setColorAt(1,QColor(30,10,24));
		scene.setBackgroundBrush(gradient);
	}
	else
	{
		scene.setBackgroundBrush(QBrush(Qt::black));
		background = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
		background->setPos(0, 0);
		background->setZValue(-1);
		toAdd.push_back(background);
	}
}

void MainWindow::updateStatusItem(int itemWidth)
{
	QFont font("Arial", itemWidth * 0.35, QFont::DemiBold);
	statusItem.setFont(font);
	statusItem.setPos(scene.width() * 0.05f, scene.height() * 0.05f);
	statusItem.setTextWidth(width() / 2);
}

MainWindow::~MainWindow()
{
	if (workerThread && workerThread->isRunning())
	{
		MainWindow::instruction_duration = 1;
		workerThread->wait(500);
	}
}

void MainWindow::updateScene()
{
	if (workerThread && !workerThread->isRunning() && !workerThread->isFinished())
	{
		workerThread->start();
	}

    QFont font("Arial", 15, QFont::DemiBold);
    while (functionCallItems.size() < functionCalls.size())
    {
        QGraphicsTextItem* item = new QGraphicsTextItem(functionCalls[functionCallItems.size()]);
        functionCallItems.push_back(item);
        toAdd.push_back(item);

        item->setPos(50 + 30 * functionCallItems.size(), 150 + 5 * functionCallItems.size());
        item->setFont(font);
        item->setDefaultTextColor(QColor(215,235,255));
    }

	while (numberItems.size() < arrays.size())
	{
		Array& array = arrays[numberItems.size()];
        numberItems.push_back(QVector<NodeGraphicsItem*>());
		for (uint i=0; i < array.size(); ++i)
		{
            NodeGraphicsItem* item = new NodeGraphicsItem(array.__get__(i));
			numberItems.last().push_back(item);
			toAdd.push_back(item);
		}
	}
	int i=0;
    for (QVector<NodeGraphicsItem*>& numbers : numberItems)
	{
		Array& array = arrays[i];
		for (uint j=0; j < array.size(); ++j)
		{
            if ((int)j >= numbers.size())
			{
                numbers.push_back(new NodeGraphicsItem(array.__get__(j)));
				// toAdd.push_back(numbers[j]);
				dirty = true;
			}
            NodeGraphicsItem* item = numbers[j];
            if (item->data() != QVariant(array.__get__(j)))
			{
                item->setData(array.__get__(j));
				dirty = true;
			}
			if (array.hasBeenReadenAt(j))
				item->displayReadenState();
			else if (array.hasBeenWrittenAt(j))
				item->displayWrittenState();
			else
				item->displayDefault();
		}
		++i;
	}

#ifdef USE_NODE
    while (treeItems.size() < nodes.size())
    {
        Node* node = nodes[treeItems.size()];
        TreeNodeGraphicsItem* item = new TreeNodeGraphicsItem(node);
        treeItems.push_back(item);
        toAdd.append(item);
    }

    for (TreeNodeGraphicsItem* item : treeItems)
    {
        if (item->needUpdate())
            dirty = true;
    }
#endif // USE_NODE

	for (QGraphicsItem* item : toAdd)
	{
		scene.addItem(item);
		dirty=true;
	}
	toAdd.clear();

	for (QPair<QGraphicsTextItem*, QString>& update : toUpdate)
	{
		update.first->setPlainText(update.second);
		dirty=true;
	}
	toUpdate.clear();

	for (QGraphicsItem* item : toRemove)
	{
		item->hide();
		dirty=true;
	}
	toRemove.clear();

	if (zoom)
	{
		dirty = true;
		qDebug() << view.mapToScene(targetPos.toPoint()) << QString::number(scale);
		view.centerOn(view.mapToScene(targetPos.toPoint()));
		view.setTransform(QTransform::fromScale(scale, scale));
		zoom = false;
	}

	if (dirty)
	{
		updateLayout();
		repaint();
	}
}

void MainWindow::pushFunctionCall(QString functionName, int parameter)
{
    functionCalls.push(QString("%1(%2)").arg(functionName).arg(QString::number(parameter)));
    std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration));
}

void MainWindow::pushFunctionCall(QString functionName, int parameter1, int parameter2)
{
    functionCalls.push(QString("%1(%2, %3)").arg(functionName)
                                            .arg(parameter1)
                                            .arg(parameter2));
    std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration));
}

void MainWindow::pushFunctionCall(QString functionName, QVector2D parameter1, int parameter2, QVector2D parameter3)
{
    functionCalls.push(QString("%1([%2, %3], %4, [%5, %6])").arg(functionName)
                                            .arg(parameter1.x())
                                            .arg(parameter1.y())
                                            .arg(parameter2)
                                            .arg(parameter3.x())
                                            .arg(parameter3.y()));
    std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration));
}

void MainWindow::pushFunctionCall(QString functionName, const Array& parameter1, int parameter2)
{
    functionCalls.push(QString("%1(array<%2 elements>, %3)").arg(functionName)
                                            .arg(parameter1.size())
                                            .arg(parameter2));
    std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration));
}

void MainWindow::pushFunctionCall(QString functionName, const Array &parameter1, const Array &parameter2, int parameter3, int parameter4)
{
    functionCalls.push(QString("%1(array<%2 elements>, array<%2 elements>, %4, %3)")
                                            .arg(functionName)
                                            .arg(parameter1.size())
                                            .arg(parameter2.size())
                                            .arg(parameter3)
                                            .arg(parameter4));
    std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration));
}

int MainWindow::popFunctionCall(int result)
{
    popFunctionCall(QString::number(result));
    return result;
}

void MainWindow::popFunctionCall(QVariant result)
{
    if (!functionCalls.empty())
    {
        if (!functionCallItems.isEmpty())
        {
            QGraphicsTextItem* item = functionCallItems.takeLast();
            while (toAdd.contains(item))
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            toUpdate.push_back(QPair<QGraphicsTextItem*, QString>(item, item->toPlainText() +
                                                                  QString(" -> %1").arg(result.toString())));
            std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration));
            toRemove.push_back(item);
        }
        functionCalls.pop();
    }
}

int MainWindow::updateFunctionCalls(int itemWidth, int &maxY, int &maxX)
{
    for (QGraphicsTextItem* item : functionCallItems)
    {
        int startX = 0.05 * width();

        item->setPos(startX, maxY);
        maxY += itemWidth * 1.2;
    }
    return maxY;
}

void MainWindow::addBinaryNode(Node *node)
{
#ifdef USE_NODE
    nodes.push_back(node);
    this->dirty = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration / 3));
#endif // USE_NODE
}

void MainWindow::handleResult()
{
	if (workerThread && workerThread->message().size())
	{
		if (workerThread->succeeded())
		{
			statusItem.setPlainText(QString("Success, gg bg: %1").arg(workerThread->message()));
			statusItem.setDefaultTextColor(QColor(100,230,100));
		}
		else
		{
			statusItem.setPlainText(QString("Fail: %1").arg(workerThread->message()));
			statusItem.setDefaultTextColor(QColor(250,50,80));
		}
		disconnect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
		workerThread->deleteLater();
		workerThread = nullptr;
	}
}

NodeGraphicsItem::NodeGraphicsItem(const int number, QGraphicsItem* parent) :
    QGraphicsEllipseItem(parent), _data(number), state(STATE_COUNT)
{
	QPen pen;
	pen.setWidth(3);
	setPen(pen);
    displayDefault();
}

NodeGraphicsItem::NodeGraphicsItem(const std::string& data, QGraphicsItem* parent) :
    QGraphicsEllipseItem(parent), _data(QString::fromStdString(data)), state(STATE_COUNT)
{
    QPen pen;
    pen.setWidth(3);
    setPen(pen);
    displayDefault();
}

NodeGraphicsItem::NodeGraphicsItem(const QVariant &data, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), _data(data), state(STATE_COUNT)
{
    QPen pen;
    pen.setWidth(3);
    setPen(pen);
    displayDefault();
}


void NodeGraphicsItem::setData(const std::string& data)
{
    setData(QVariant(QString::fromStdString(data)));
}

void NodeGraphicsItem::setData(const int data)
{
    setData(QVariant(data));
}

void NodeGraphicsItem::setData(const QVariant& data)
{
    if(this->_data != data)
	{
        this->_data = data;
		displayDefault(true);
		update();
	}
}

void NodeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
							   QWidget *widget)
{
	QGraphicsEllipseItem::paint(painter, option, widget);
	painter->setFont(widget->parentWidget()->font());
    painter->drawText(this->rect(), Qt::AlignCenter, _data.toString());
}

void NodeGraphicsItem::displayDefault()
{
	displayDefault(false);
}

void NodeGraphicsItem::displayDefault(bool force)
{
	if (state == DEFAULT && !force)
		return;
    state = DEFAULT;

    bool ok = false;
    int _number = _data.toInt(&ok);
    if (ok)
    {
        QPen pen(this->pen());
        pen.setColor(QColor(150,220,183).lighter(60+80.f*_number/255.f));
        setPen(pen);
    }
    else
    {
        QPen pen(this->pen());
        pen.setColor(QColor(150,220,183));
        setPen(pen);
    }
}

void NodeGraphicsItem::displayReadenState()
{
	if (state == READEN)
		return;
    state = READEN;

    bool ok = false;
    int _number = _data.toInt(&ok);
    if (!ok)
        return;

	QPen pen(this->pen());
	pen.setColor(QColor(255,20,100).lighter(100+20.f*_number/500.f));
	setPen(pen);
}
void NodeGraphicsItem::displayWrittenState()
{
	if (state == WRITEN)
		return;
    state = WRITEN;

    bool ok = false;
    int _number = _data.toInt(&ok);
    if (!ok)
        return;

    QPen pen(this->pen());
	pen.setColor(QColor(150,150,255).lighter(100+20.f*_number/255.f));
	setPen(pen);
}

QVariant NodeGraphicsItem::data() const
{
    return _data;
}


#ifdef USE_NODE
TreeNodeGraphicsItem::TreeNodeGraphicsItem(const Node* node, QGraphicsItem* parent,
                                               float leftShifting, float rightShifting)
    : Base(node->toString(), parent), _node(node), left(nullptr), right(nullptr),
      leftShifting(leftShifting), rightShifting(rightShifting)
{}

void TreeNodeGraphicsItem::setNode(const Node *node)
{
    this->_node = node;

    if (node)
        setData(node->toString());
    else
        setData(0);
}

const Node *TreeNodeGraphicsItem::node() const
{
    return _node;
}

int TreeNodeGraphicsItem::width() const
{
    int width = rect().width() * (1.f+qMax(rightShifting,leftShifting));
    int childrenWidth = 0;
    if (left)
        childrenWidth = left->width();
    if (right)
        childrenWidth = qMax(childrenWidth, right->width());
    return width + childrenWidth;
}

int TreeNodeGraphicsItem::height() const
{
    int height = this->rect().width() * (1+this->_treeHeight());
    int childrenheight = 0;
    if (left)
        childrenheight = left->height();
    if (right)
        childrenheight = qMax(childrenheight, right->height());
    return height + childrenheight;
}

bool TreeNodeGraphicsItem::needUpdate() const
{
    if (data() != QVariant(_node->toString()))
        return true;

    if (_node->leftChild() && !left)
        return true;

    if (_node->rightChild() && !right)
        return true;

    if (left && left->needUpdate())
        return true;

    if (right && right->needUpdate())
        return true;
    return false;
}

int TreeNodeGraphicsItem::_treeHeight() const
{
    int height = 1;
    if (left)
        height = 1 + left->_treeHeight();
    if (right)
        height = qMax(height, 1 + right->_treeHeight());
    return height;
}

void TreeNodeGraphicsItem::updateNode()
{
    if (!parentItem())
    {
        if (right)
            rightShifting = right->_treeHeight() + 1;

        if (left)
            leftShifting = left->_treeHeight() + 1;
    }
    if (data() != QVariant(_node->toString()))
        setData(_node->toString());
    if (_node->leftChild())
    {
        if (!left)
            left = new TreeNodeGraphicsItem(_node->leftChild(), this);
        left->leftShifting = left->_treeHeight();
        left->rightShifting = (rightShifting / _treeHeight()) / 2 ;
        left->updateNode();
    }
    if (_node->rightChild())
    {
        if (!right)
            right = new TreeNodeGraphicsItem(_node->rightChild(), this);
        right->leftShifting = (leftShifting / _treeHeight()) / 2;
        right->rightShifting = right->_treeHeight();
        right->updateNode();
    }
}

void TreeNodeGraphicsItem::updateLayout()
{
    const QRectF& rect = this->rect();
    int width = rect.width();

    if (left)
    {
        if (this->scene() && !left->scene())
            this->scene()->addItem(left);
        left->setRect(rect.x() - width * leftShifting, rect.y() +
                      width  * (2+left->_treeHeight()) / 2.f, width, width);
        left->updateLayout();
    }
    if (right)
    {
        if (this->scene() && !right->scene())
            this->scene()->addItem(right);
        right->setRect(rect.x() + width * rightShifting, rect.y() +
                       width * (2+right->_treeHeight()) / 2.f, width, width);
        right->updateLayout();
    }
    update();
}

QRectF TreeNodeGraphicsItem::boundingRect() const
{
    const QRectF& rect = this->rect();
    int treeHeight = this->_treeHeight();
    int width = rect.width();
    int totalWidth = width * (1.f+rightShifting+leftShifting);
    return QRectF(rect.x() - totalWidth, rect.y(), totalWidth * 2, width * (1+treeHeight) / 2);
}

void TreeNodeGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Base::paint(painter, option, widget);
    const QRectF& rect = this->rect();
    if (left)
    {
        const QRectF& rectl = left->rect();
        painter->drawLine(rect.x()+rect.width()*0.5,rect.y() + rect.height(), rectl.x()+rectl.width()*0.5, rectl.y());
    }
    if (right)
    {
        const QRectF& rectr = right->rect();
        painter->drawLine(rect.x()+rect.width()*0.5, rect.y() + rect.height(), rectr.x()+rectr.width()*0.5, rectr.y());
    }
}
#endif
