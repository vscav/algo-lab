#ifndef TP6_H
#define TP6_H

#include <QGraphicsLineItem>

#include "array.h"
#include "graph.h"
#include "mainwindow.h"
#include "thread.h"

class GraphThread : public Thread
{

public:
	GraphThread(MainWindow* mainWindow, Graph& graph, QObject *parent = nullptr)
		: Thread(parent), mainWindow(mainWindow), success(false), graph(graph) {}

	virtual bool succeeded() const {return success;}
	virtual const QString& message() const {return _message;}
	virtual ~GraphThread() {}

protected:
	MainWindow* mainWindow;
	QString _message;
	bool success;

	Graph& graph;
};

class GraphBuildThread : public GraphThread
{

public:
	GraphBuildThread(MainWindow* mainWindow, Graph& graph, QObject *parent = nullptr)
		: GraphThread(mainWindow, graph, parent) {}
	void run() override;

	~GraphBuildThread() {}
};

class GraphDeepTravelThread : public GraphThread
{

public:
	GraphDeepTravelThread(MainWindow* mainWindow, Graph& graph, QObject *parent = nullptr)
		: GraphThread(mainWindow, graph, parent) {}
	void run() override;

	~GraphDeepTravelThread() {}
};

class GraphWideTravelThread : public GraphThread
{

public:
	GraphWideTravelThread(MainWindow* mainWindow, Graph& graph, QObject *parent = nullptr)
		: GraphThread(mainWindow, graph, parent) {}
	void run() override;

	~GraphWideTravelThread() {}
};

class GraphCycleDetectionThread : public GraphThread
{

public:
	GraphCycleDetectionThread(MainWindow* mainWindow, Graph& graph, QObject *parent = nullptr)
		: GraphThread(mainWindow, graph, parent) {}
	void run() override;

	~GraphCycleDetectionThread() {}
};

class EdgeGraphicsItem : public QGraphicsLineItem
{
public:
	typedef QGraphicsLineItem Base;

	explicit EdgeGraphicsItem(const QGraphicsEllipseItem* source, const QGraphicsEllipseItem* destination,
							  int distance, QGraphicsItem* parent=nullptr);
	void setDistance(int distance);
	void setPath(const QGraphicsEllipseItem *source, const QGraphicsEllipseItem *destination);
	const QGraphicsEllipseItem* sourceItem();
	const QGraphicsEllipseItem* destinationItem();

	void updateLayout();

	QPainterPath painterPath() const;
	QRectF boundingRect() const;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget=nullptr) override;
	int distance() const;

	static QColor baseColor;

private:
	QString text;
	int _distance;
	const QGraphicsEllipseItem* source;
	const QGraphicsEllipseItem* destination;
};

class GraphGraphicsItem : public QGraphicsEllipseItem
{
public:
	typedef QGraphicsEllipseItem Base;

	explicit GraphGraphicsItem(const GraphNode* node, QGraphicsItem* parent=nullptr);
	explicit GraphGraphicsItem(const GraphNode* node, QGraphicsScene* scene);
	void setNode(const GraphNode* node);

	void paint(QPainter* painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget=nullptr) override;
	const GraphNode *node() const;

	static QColor baseColor;

private:
	QString text;
	const GraphNode* _node;
	std::list<EdgeGraphicsItem*> edges;
};

class GraphWindow : public MainWindow
{
public:
	typedef MainWindow Base;

	GraphWindow(QWidget *parent=nullptr)
		: MainWindow(parent), graph(nullptr)
	{
		qsrand(time(nullptr));
		int n = qrand() % 8 + 5;
		this->newGraph(n);

		threads.push_back(new GraphBuildThread(this, *graph, this));
		threads.push_back(new GraphDeepTravelThread(this, *graph, this));
		threads.push_back(new GraphWideTravelThread(this, *graph, this));
		threads.push_back(new GraphCycleDetectionThread(this, *graph, this));

		currentThread = threads.begin();
		workerThread = *currentThread;
		connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
	}

	Graph& newGraph(int size);

	int updateGraphItems(int itemWidth, int &maxY, int &maxX);

	void updateLayout() override;
	void handleResult() override;
	void updateScene() override;

private:
	QVector<GraphGraphicsItem *> graphItems;
	QVector<QVector<EdgeGraphicsItem *> > edgeItems;
	Graph* graph;
	QLinkedList<Thread*> threads;
	QLinkedList<Thread*>::iterator currentThread;
};

#endif // TP6_H
