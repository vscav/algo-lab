#include "tp2.h"
#include "math.h"
void SumThread::run() {
	try
	{
		qsrand(time(nullptr));

		int n = qrand() % 10 + 5;
        int result = function(n);
		_assert(n, result);
		_message = QString("Sum of 1..%1 is %2").arg(n).arg(result);
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
    }
}

void SumThread::_assert(int param, int result) const
{
	int sum=0;
	for (int i=0;i<=param;++i) {
		sum += i;
	}
	if (result != sum)
	{
		QString message("Sum of 1..%1 failed: %2 has been processed but it should be %3");
		throw std::runtime_error(message.arg(param)
										.arg(result)
										.arg(sum)
										.toStdString());
	}
}

void PowerThread::run() {
	try
	{
		qsrand(time(nullptr));

		int value = qrand() % 15;
		int n = qrand() % 8;
		int result = function(value, n);
		_assert(value, n, result);
		_message = QString("Power %1 of %2 is %3").arg(value).arg(n).arg(result);
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

void PowerThread::_assert(int value, int param, int result) const
{
	int power=pow(value, param);
	if (result != power)
	{
		QString message("Power %1 of %2 failed: %3 has been processed but it should be %4");
		throw std::runtime_error(message.arg(param)
										.arg(value)
										.arg(result)
										.arg(power)
										.toStdString());
	}
}

void FibonacciThread::run()
{
	try
	{
		qsrand(time(nullptr));

		int n = qrand() % 10 + 5;
		int result = function(n);
		_assert(n, result);
		_message = QString("Fibonacci %1 is %2").arg(n).arg(result);
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

void FibonacciThread::_assert(int n, int result) const
{
	int fibo1 = 0;
	int fibo2 = 1;
	int fibo = fibo1;
	if (n == 1)
		fibo = 1;
	else if (n > 1)
	{
		for (int i=1; i<n; ++i)
		{
			fibo=fibo1+fibo2;
			fibo1=fibo2;
			fibo2=fibo;
		}
	}
	if (result != fibo)
	{
		QString message("Fibonacci %1 failed: %2 has been processed but it should be %3");
		throw std::runtime_error(message.arg(n)
										.arg(result)
										.arg(fibo)
										.toStdString());
	}
}


void SearchThread::run()
{
	try
	{
		qsrand(time(nullptr));

		int n = qrand() % 10 + 5;
		Array& a = mainWindow->newRandomArray(n);
		int index = qrand() % a.size() - 1;
		int toSearch = -1;
		if (index >= 0)
			toSearch = a.__get__(index);
		int result = function(toSearch, a, a.size());

		if (index >= 0)
		{
			if (a.__get__(result) == toSearch)
			{
				_message = QString("%1 is at index %2\nArray was:").arg(toSearch).arg(result);
				success = true;
			}
		}
		else
		{
			if (result < 0)
			{
				_message = QString("%1 is not in the array\nArray was:").arg(toSearch);
				success = true;
			}
		}
		if (!success)
		{
			QString message("Search failed: %1 has been found but it should be %2\nArray was: ");
			message.append(a.toString());
			throw std::runtime_error(message.arg(result)
											.arg(index)
											.toStdString());
		}
		_message.append(a.toString());
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

void AllEvensThread::run()
{
	try
	{
		qsrand(time(nullptr));

		int n = qrand() % 10 + 5;
		Array& a = mainWindow->newRandomArray(n);
		Array& result = mainWindow->newArray(n);
		function(result, a, 0, a.size());
		_assert(a, result);
		_message = QString("%1 evens found\nArray was:").arg(result.effectiveSize());
		_message.append(a.toString());
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

void AllEvensThread::_assert(const Array &origin, const Array &result) const
{
	for (uint i=0; i<origin.size(); ++i)
	{
		if (origin.__get__(i) % 2 == 0)
		{
			if (!result.contains(origin.__get__(i)))
				throw std::runtime_error(QString("%1 is an even number but it's not in result\nArray was %2")
										 .arg(origin.__get__(i))
										 .arg(origin.toString())
											  .toStdString());
		}
		else
		{
			if (result.contains(origin.__get__(i)))
				throw std::runtime_error(QString("%1 is an odd number but it's in result\nArray was %2")
										 .arg(origin.__get__(i))
										  .arg(origin.toString())
											  .toStdString());
		}
	}
}

void MandelbrotThread::run()
{
	qsrand(time(nullptr));

	QImage* mandelbrotPicture = new QImage(2048, 2048, QImage::Format_RGB32);
	double n = qrand() % 80 + 100;
	QRgb white = qRgb(qrand() % 125 + 50, qrand() % 125 + 75, qrand() % 125 + 50);
	QRgb black = qRgb(qrand() % 20, qrand() % 30, qrand() % 20);
	QVector2D z((qrand() % 200 - 100) * 0.0002, (qrand() % 200 - 100) * 0.0002);

    double factor = 0.5 + (qrand() % 30000 - 15000) * 0.0001;
	double precision = 100000. / factor;
	double width = mandelbrotPicture->width() * precision * 0.25, height = mandelbrotPicture->height() * precision * 0.5;


	for (int i=0; i<mandelbrotPicture->width(); ++i)
		for (int j=0; j<mandelbrotPicture->height(); ++j)
		{
			QVector2D point(factor * (-2.5 + (precision * (double)(i)) / width) - 0.7 * (1-factor),
							factor * ((-.5 + (precision * (double)(j)) / height)) - 0.4 * (1-factor));
			double iter = function(z, n, point);
			if (iter > 0.f)
			{
				QColor color(white);
				mandelbrotPicture->setPixel(i, j, color.darker(200. * iter / n).rgb());
			}
			else
			{
				mandelbrotPicture->setPixel(i, j, black);
			}
		}
	mainWindow->setBackground(mandelbrotPicture);
    success=true;
	_message = QString("Mandebrot: n=%1 z=[%2, %3] factor=%4").arg(n).arg(z.x()).arg(z.y()).arg(factor);
}

