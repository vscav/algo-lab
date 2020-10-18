#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <functional>
#include <stdexcept>

#include <qglobal.h>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QLinkedList>
#include <QVector2D>
#include <QStack>
#include <QTimer>

#include "thread.h"
#include "array.h"
#ifdef USE_NODE
#include "binarytree.h"
#else
class Node {};
#endif // USE_NODE


using std::size_t;

class NodeGraphicsItem : public QGraphicsEllipseItem
{
public:
    explicit NodeGraphicsItem(const int _number, QGraphicsItem* parent=nullptr);
    explicit NodeGraphicsItem(const std::string& data, QGraphicsItem* parent=nullptr);
    explicit NodeGraphicsItem(const QVariant& data, QGraphicsItem* parent=nullptr);

    void setData(const QVariant& data);
    void setData(const int _number);
    void setData(const std::string& data);

	void paint(QPainter* painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget=nullptr) override;

	void displayDefault();
	void displayReadenState();
	void displayWrittenState();
    QVariant data() const;

private:
	enum State
	{
		DEFAULT=0,
		READEN,
		WRITEN,
		STATE_COUNT
	};
	void displayDefault(bool force);

    QVariant _data;
	State state;
};


class TreeNodeGraphicsItem : public NodeGraphicsItem
{
public:
    typedef	NodeGraphicsItem Base;

    explicit TreeNodeGraphicsItem(const Node *node, QGraphicsItem* parent=nullptr, float leftShiftting=0, float rightShiftting=0);
    void setNode(const Node* node);
    const Node* node() const;

    int width() const;
    int height() const;


    int _treeHeight() const;
    virtual bool needUpdate() const;

    virtual void updateNode();
    void updateLayout();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget=nullptr) override;


public:
    const Node* _node;

    TreeNodeGraphicsItem* left;
    TreeNodeGraphicsItem* right;

    float leftShifting=0;
    float rightShifting=0;
};


class MainWindow : public QMainWindow
{

	Q_OBJECT

public:
    typedef QMainWindow Base;

    static MainWindow* _instance;
    static MainWindow* instance();

	explicit MainWindow(QWidget* parent=nullptr);

	Array& mainArray() {return arrays[0];}
	Array& newRandomArray(uint size);
	Array& newSortedRandomArray(uint size);
	Array& newArray(uint size);

    void addBinaryNode(Node *node);

    void resizeEvent(QResizeEvent*) override;
    void closeEvent(QCloseEvent*)  override;

	void setBackground(QImage* image);

    virtual void updateLayout();
    virtual void updateLayoutItems(int itemWidth, int &originX, int &originY);

    void updateStatusItem(int itemWidth);
	void updateBackground();
	int updateNumberItems(int itemWidth, int &maxY, int &maxX);
    void updateTreeItems(int itemWidth, int &maxY, int &maxX);

    void pushFunctionCall(QString functionName, int parameter);
    void pushFunctionCall(QString functionName, int parameter1, int parameter2);
    void pushFunctionCall(QString functionName, QVector2D parameter1, int parameter2, QVector2D parameter3);
    void pushFunctionCall(QString functionName, const Array& parameter1, int parameter2);
    void pushFunctionCall(QString functionName, const Array& parameter1, const Array& parameter2, int parameter3, int parameter4);
    int popFunctionCall(int result);
    void popFunctionCall(QVariant result);

    int updateFunctionCalls(int itemWidth, int &maxY, int &maxX);

	static unsigned long instruction_duration;

	bool eventFilter(QObject* object, QEvent* event);
    void showEvent(QShowEvent* event) override;

	~MainWindow();
public slots:
	virtual void updateScene();
	virtual void handleResult();

private:

	void initialize();

	QGraphicsView view;

    QVector<QVector<NodeGraphicsItem*> > numberItems;
	QVector<QGraphicsTextItem *> accessInfoItem;
	QGraphicsTextItem statusItem;

    QStack<QString> functionCalls;
    QLinkedList<QGraphicsTextItem* > functionCallItems;

    QVector<Array> arrays;
    QVector<Node*> nodes;

    QLinkedList<TreeNodeGraphicsItem* > treeItems;

	QTimer timer;

	QGraphicsPixmapItem* background;

	bool zoom;
	float scale;
	QPointF targetPos;

protected:
	QGraphicsScene scene;

	QVector<QGraphicsItem*> toAdd;
	QVector<QPair<QGraphicsTextItem*, QString> > toUpdate;
	QVector<QGraphicsItem*> toRemove;

	Thread* workerThread;
	bool dirty;
};


#endif // MAINWINDOW_H
