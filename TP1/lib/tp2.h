#ifndef TP2_H
#define TP2_H

#include <QStack>

#include "mainwindow.h"
#include "thread.h"

class SumThread : public TestThread<std::function<int(int)> >
{
	public:
	SumThread(MainWindow* mainWindow, ThreadFunctionType function, QObject *parent = nullptr)
		: TestThread<std::function<int(int)> >(mainWindow, function, parent) {}
	void run() override;
	void _assert(int param, int result) const;
};


class PowerThread : public TestThread<std::function<int(int, int)> >
{
	public:
	PowerThread(MainWindow* mainWindow, ThreadFunctionType function, QObject *parent = nullptr)
		: TestThread<ThreadFunctionType>(mainWindow, function, parent) {}
	void run() override;
	void _assert(int value, int power, int result) const;
};


class FibonacciThread : public TestThread<std::function<int(int)> >
{
	public:
	FibonacciThread(MainWindow* mainWindow, ThreadFunctionType function, QObject *parent = nullptr)
		: TestThread<ThreadFunctionType>(mainWindow, function, parent) {}
	void run() override;
	void _assert(int n, int result) const;
};


class SearchThread
		: public TestThread<std::function<int(int, Array&, int)> >
{

	public:
	SearchThread(MainWindow* mainWindow, ThreadFunctionType function, QObject *parent = nullptr)
		: TestThread<ThreadFunctionType>(mainWindow, function, parent) {}
	void run() override;
};


class AllEvensThread
		: public TestThread<std::function<void(Array&, Array&, int, int)> >
{
	public:
	AllEvensThread(MainWindow* mainWindow, ThreadFunctionType function, QObject *parent = nullptr)
		: TestThread<ThreadFunctionType>(mainWindow, function, parent) {}

	void run() override;
	void _assert(const Array& origin, const Array& result) const;
};


class MandelbrotThread : public TestThread<std::function<int(QVector2D, int, QVector2D)> >
{
	public:
	MandelbrotThread(MainWindow* mainWindow, ThreadFunctionType function, QObject *parent = nullptr)
		: TestThread<ThreadFunctionType>(mainWindow, function, parent) {}
	void run() override;
};

template <class ThreadType>
class TestMainWindow : public MainWindow
{

public:
	typedef ThreadType WindowThreadType;
	typedef typename ThreadType::ThreadFunctionType FunctionType;

	TestMainWindow(FunctionType function, QWidget *parent=nullptr)
        : MainWindow(parent)
	{
		workerThread = new WindowThreadType(this, function, this);
		connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
	}
};

class SumWindow : public TestMainWindow<SumThread>
{
	Q_OBJECT

public:
	SumWindow(SumThread::ThreadFunctionType function, QWidget* parent=nullptr)
		: TestMainWindow<SumThread>(function, parent) {}
};


class PowerWindow : public TestMainWindow<PowerThread>
{
	Q_OBJECT

public:
	PowerWindow(PowerThread::ThreadFunctionType function, QWidget* parent=nullptr)
		: TestMainWindow<PowerThread>(function, parent) {}
};

class FibonacciWindow : public TestMainWindow<FibonacciThread>
{
	Q_OBJECT

public:
	FibonacciWindow(FibonacciThread::ThreadFunctionType function, QWidget* parent=nullptr)
		: TestMainWindow<FibonacciThread>(function, parent) {}
};


class SearchWindow : public TestMainWindow<SearchThread>
{
	Q_OBJECT

public:
	SearchWindow(SearchThread::ThreadFunctionType function, QWidget* parent=nullptr)
		: TestMainWindow<SearchThread>(function, parent) {}
};


class AllEvensWindow : public TestMainWindow<AllEvensThread>
{
	Q_OBJECT

public:
	AllEvensWindow(AllEvensThread::ThreadFunctionType function, QWidget* parent=nullptr)
		: TestMainWindow<AllEvensThread>(function, parent) {}
};


class MandelbrotWindow : public TestMainWindow<MandelbrotThread>
{
	Q_OBJECT

public:
	MandelbrotWindow(MandelbrotThread::ThreadFunctionType function, QWidget* parent=nullptr)
		: TestMainWindow<MandelbrotThread>(function, parent) {}
};

struct Context{

    Context(QString func_name, int param){
        if (MainWindow::instance())
        {
            MainWindow::instance()->pushFunctionCall(func_name, param);
        }
    }

    Context(QString func_name, int param1, int param2){
        if (MainWindow::instance())
        {
            MainWindow::instance()->pushFunctionCall(func_name, param1, param2);
        }
    }

    template<typename T>
    T store(T result)
    {
        this->result = result;
        return result;
    }

    ~Context(){
        if (MainWindow::instance())
        {
            MainWindow::instance()->popFunctionCall(result);
        }
    }

    QVariant result;
};

#endif // TP2_H
