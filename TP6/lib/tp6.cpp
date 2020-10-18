#include "tp6.h"
#include <math.h>

Graph &GraphWindow::newGraph(int size)
{
	graph = new Graph(size);
	return *graph;
}

int GraphWindow::updateGraphItems(int itemWidth, int &maxY, int &maxX)
{
	srand(100);
	int X, Y;
	int centerX = width() * 0.5, centerY = height() * 0.5;
	int distance = height() * 0.25;
	float x = 0;
	for (GraphGraphicsItem* item : graphItems)
	{
		X = centerX + cos(x) * distance + itemWidth * -2 + qrand() % ((int)(width() * 0.1f));
		Y = centerY + sin(x) * distance + itemWidth * -2 + qrand() % ((int)(height() * 0.1f));

		if (Y < 0)
			Y = qrand() % ((int)(height() * 0.1f));
		if (X < 0)
			X = qrand() % ((int)(width() * 0.1f));

		item->setRect(X, Y, itemWidth, itemWidth);

		if (X > maxX)
			maxX = X;

		if (Y > maxY)
			maxY = Y;

		x += .7f;
		distance += height() * 0.04f;
	}

	for (const QVector<EdgeGraphicsItem*>& edges : edgeItems)
	{
		for (EdgeGraphicsItem* edge : edges)
		{
			edge->updateLayout();
		}
	}
	return maxY;
}

void GraphWindow::updateLayout()
{
	if (dirty)
	{
		int itemWidth = qMax<int>(50, width() * 0.01f);
		int maxX=0, maxY=0;

		updateStatusItem(itemWidth);
		updateBackground();
		updateGraphItems(itemWidth, maxX, maxY);

		this->scene.setSceneRect(0, 0, qMax(width(), maxX), qMax(height(),maxY+itemWidth * 2));
	}
	this->dirty=false;
}

void GraphWindow::handleResult()
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

void GraphWindow::updateScene()
{
	Base::updateScene();
	while (graphItems.size() < graph->nodesCount())
	{
		GraphNode& node = (*graph)[graphItems.size()];
		GraphGraphicsItem* newItem = new GraphGraphicsItem(&node);
		graphItems.push_back(newItem);
		edgeItems.push_back(QVector<EdgeGraphicsItem *>());
		toAdd.push_back(newItem);
	}
	int i=0;
	for (GraphGraphicsItem*& item : graphItems)
	{
		GraphNode& node = (*graph)[i];
		QVector<EdgeGraphicsItem* >& edges = edgeItems[i];
		int j=0;
		for (Edge* edge=node.edges; edge != nullptr; edge=edge->next)
		{
			if (j >= edges.size())
			{
				GraphGraphicsItem* source = graphItems[edge->source->value];
				GraphGraphicsItem* destination = graphItems[edge->destination->value];
				edges.push_back(new EdgeGraphicsItem(source, destination, edge->distance));
				toAdd.push_back(edges[j]);
				dirty = true;
			}
			else
			{
				EdgeGraphicsItem* edgeItem = edges[j];
				if (edgeItem->sourceItem() != graphItems[edge->source->value] ||
						edgeItem->destinationItem() != graphItems[edge->destination->value])
				{
					GraphGraphicsItem* source = graphItems[edge->source->value];
					GraphGraphicsItem* destination = graphItems[edge->destination->value];
					edgeItem->setPath(source, destination);
					edgeItem->setDistance(edge->distance);
					dirty = true;
				}
			}
			j++;
		}
		++i;
	}
}

QColor EdgeGraphicsItem::baseColor(50, 120, 93);

EdgeGraphicsItem::EdgeGraphicsItem(const QGraphicsEllipseItem *source, const QGraphicsEllipseItem *destination,
								   int distance, QGraphicsItem *parent)
	: QGraphicsLineItem(parent), source(source), destination(destination)
{
	setDistance(distance);
}

void EdgeGraphicsItem::setDistance(int distance)
{
	this->text = QString::number(distance);
	this->_distance = distance;
	QPen pen;
	pen.setWidth(4);
	pen.setColor(baseColor.lighter(255 * ((float)_distance / 30.f)));
	setPen(pen);
}

void EdgeGraphicsItem::setPath(const QGraphicsEllipseItem *source, const QGraphicsEllipseItem *destination)
{
	this->source = source;
	this->destination = destination;
}

const QGraphicsEllipseItem *EdgeGraphicsItem::sourceItem()
{
	return this->source;
}

const QGraphicsEllipseItem *EdgeGraphicsItem::destinationItem()
{
	return this->destination;
}

void EdgeGraphicsItem::updateLayout()
{
	float dest_x = destination->rect().center().x();
	float source_x = source->rect().left();
	if (dest_x > source_x)
		source_x = source->rect().right();
	float source_y = source->rect().center().y();
	float dest_y = destination->rect().top();
	if (source_y > dest_y)
		dest_y = destination->rect().bottom();

	this->setLine(source_x,source_y, dest_x, dest_y);
}

QPainterPath EdgeGraphicsItem::painterPath() const
{
	QPainterPath path(this->line().p1());
	float length = this->line().length();
	length = qMax<float>(length, source->rect().width() * 2) * 0.35f;

	QLineF normal = this->line().normalVector().unitVector();
	QPointF normalP = QPointF(normal.dx(), normal.dy()) * length;
	if (this->line().p1().y() > this->line().p2().y())
		normalP *= -1;
	path.cubicTo(this->line().p1(),
				 (this->line().p1() * 0.4f + this->line().p2() * 0.6f) + normalP,
				 this->line().p2());
	return path;
}

QRectF EdgeGraphicsItem::boundingRect() const
{
	return painterPath().boundingRect();
}

void EdgeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

	painter->setPen(this->pen());
	painter->drawPath(painterPath());
	QLineF normalVector = this->line().normalVector().unitVector();
	QLineF unit = this->line().unitVector();
	QPointF normalPoint(normalVector.dx() * 0.3f + unit.dx() * 0.7f,
						normalVector.dy() * 0.3f + unit.dy() * 0.7f);
	normalPoint *= this->source->rect().width() * -0.5f;
	QPointF arrowFirst = this->line().p2() + normalPoint;
	normalPoint = QPointF(-normalVector.dx() * 0.3f + unit.dx() * 0.7f,
						  -normalVector.dy() * 0.3f + unit.dy() * 0.7f);
	normalPoint *= this->source->rect().width() * -0.5f;
	QPointF arrowSecond = this->line().p2() + normalPoint;
	painter->drawLine(this->line().p2(), arrowFirst);
	painter->drawLine(this->line().p2(), arrowSecond);
	painter->drawText(this->boundingRect(), Qt::AlignCenter, text);
}

int EdgeGraphicsItem::distance() const
{
	return _distance;
}

QColor GraphGraphicsItem::baseColor(150, 220, 183);

GraphGraphicsItem::GraphGraphicsItem(const GraphNode *node, QGraphicsItem *parent)
	: QGraphicsEllipseItem(parent)
{
	this->setNode(node);
	QPen pen;
	pen.setWidth(3);
	pen.setColor(baseColor);
	setPen(pen);
}

void GraphGraphicsItem::setNode(const GraphNode *node)
{
	this->_node = node;
	text = QString::number(node->value);
}

void GraphGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
							  QWidget *widget)
{
	Base::paint(painter, option, widget);
	painter->setFont(widget->parentWidget()->font());
	painter->drawText(this->rect(), Qt::AlignCenter, text);
}

const GraphNode* GraphGraphicsItem::node() const
{
	return _node;
}

void GraphBuildThread::run()
{
	qsrand(time(nullptr));
	int nodeCount = qrand() % 5 + 5;
	int** matrix = new int*[nodeCount];
	for (int i=0; i<nodeCount; ++i)
	{
		matrix[i] = new int[nodeCount];
		for (int j=0; j<nodeCount; ++j)
		{
			matrix[i][j] = (qrand() % 300 - 230) / 2;
			if (matrix[i][j] < 0)
				matrix[i][j] = 0;
		}
	}
	this->graph.buildFromAdjenciesMatrix(matrix, nodeCount);
	this->success = true;
}

void GraphCycleDetectionThread::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	this->_message = QString("Graph has cycle: %1").arg(graph.detectCycle());
	this->success = true;
}

void GraphWideTravelThread::run()
{
	if (!graph.nodesCount())
	{
		this->success = false;
		this->_message = QString("The graph is not built !");
		return;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	srand(time(nullptr));
	int nodeIndex = qrand() % graph.nodesCount();
	GraphNode* first = &graph[nodeIndex];

	int nodeSize = 0;
	GraphNode** nodes = new GraphNode*[graph.nodesCount()];
	bool* visited = new bool[graph.nodesCount()];

	memset(nodes, 0, sizeof(GraphNode*) * graph.nodesCount());
	memset(visited, 0, sizeof(bool) * graph.nodesCount());

	graph.wideTravel(first, nodes, nodeSize, visited);

	this->_message = QString("Graph wide travel starting by ");
	this->_message += QString(first->toString().c_str()) + QString(":\n");
	QStringList nodesString;
	for (int i=0; i<nodeSize; i++)
	{
		nodesString.push_back(QString(nodes[i]->toString().c_str()));
	}
	this->_message += nodesString.join(QString(" - "));
	this->success = true;
	delete nodes;
}

void GraphDeepTravelThread::run()
{
	if (!graph.nodesCount())
	{
		this->success = false;
		this->_message = QString("The graph is not built !");
		return;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	srand(time(nullptr));
	int nodeIndex = qrand() % graph.nodesCount();
	GraphNode* first = &graph[nodeIndex];

	int nodeSize = 0;
	GraphNode** nodes = new GraphNode*[graph.nodesCount()];
	bool* visited = new bool[graph.nodesCount()];

	memset(nodes, 0, sizeof(GraphNode*) * graph.nodesCount());
	memset(visited, 0, sizeof(bool) * graph.nodesCount());

	graph.deepTravel(first, nodes, nodeSize, visited);

	this->_message = QString("Graph deep travel starting by ");
	this->_message += QString(first->toString().c_str()) + QString(":\n");
	QStringList nodesString;
	for (int i=0; i<nodeSize; i++)
	{
		nodesString.push_back(QString(nodes[i]->toString().c_str()));
	}
	this->_message += nodesString.join(QString(" - "));
	this->success = true;
	delete nodes;
}
