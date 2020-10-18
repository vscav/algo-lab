#include "tp1.h"

void SorterThread::run() {
	if (this->isFinished())
		return;
	try
	{
		Array& arrayToSort = mainWindow->mainArray();
		this->function(arrayToSort);
		assertSort(arrayToSort);
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
		success = false;
	}
	success = true;
	_message = QString("Sorting succeed !");
}

void SorterThread::assertSort(const Array& array) const
{
	for (uint i=0; i<array.size()-1; ++i)
		if (array.__get__(i) > array.__get__(i+1))
		{
			QString message("Sorting failed: %1 > %2");
			throw std::runtime_error(message.arg(array.__get__(i))
										.arg(array.__get__(i+1))
										.toStdString());
		}
}

const QString &SorterThread::errorMessage() const
{
	return _message;
}

TestMainWindow::TestMainWindow(std::function<void (Array &)> sortFunction,
							   uint elementCount, QWidget *parent)
	: MainWindow(parent)
{
	newRandomArray(elementCount);

	workerThread = new SorterThread(this, sortFunction, this);
	connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
}
