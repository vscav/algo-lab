#include "array.h"

#include <time.h>
#include <thread>
#include <QThread>

#include "mainwindow.h"

bool AbstractArray::wait_for_operations = true;


Array::Array(uint size)
    : _readAccess(0), _writeAccess(0)
{
	resize(size);
}

Array::Array(const Array &other)
    : TemplateArray<int>(other),
	  _hasBeenReaden(other._hasBeenReaden.begin(),other._hasBeenReaden.end()),
	  _hasBeenWritten(other._hasBeenWritten.begin(),other._hasBeenWritten.end()),
	  _readAccess(other._readAccess),
	  _writeAccess(other._writeAccess)
{}

Array::Array(const std::vector<int> &other)
    : TemplateArray(other)
{
	resize(other.size());
}

void Array::resize(uint size)
{
	int oldSize = _data.size();
	if (size>0)
	{
		_data.resize(size);
		_hasBeenReaden.resize(size);
		_hasBeenWritten.resize(size);
	}
	for (uint i=oldSize; i<size; ++i)
        _data[i] = -1;
}

int Array::effectiveSize() const
{
	int size=0;
	for (int value : _data)
		if (value < 0)
			return size;
		else
			size++;
	return size;
}

QString Array::toString() const
{
	QStringList list;
	for (int value : _data)
	{
		list.append(QString::number(value));
	}
	return QString("[%1]").arg(list.join(", "));
}

const int& Array::get(const uint index) const
{

    const_cast<Array*>(this)->_hasBeenReaden[index] = true;
    const_cast<Array*>(this)->_readAccess++;
    return Base::get(index);
}

void Array::set(const uint index, int value)
{
	_hasBeenWritten[index] = true;
	_writeAccess++;
    Base::set(index, value);
}

void Array::swap(const uint index1, const uint index2)
{
	_hasBeenWritten[index1] = _hasBeenWritten[index2] = true;
	_writeAccess += 2;
	_hasBeenReaden[index1] = _hasBeenReaden[index2] = true;
	_readAccess += 2;
    Base::swap(index1, index2);
}

bool Array::contains(int value) const
{
	for (int v : _data)
		if (v == value)
			return true;
	return false;
}

void Array::insert(const uint index, int value)
{
	uint i=index;
	while(i < _data.size()-1 && _data[i] >= 0)
		i++;
    _writeAccess+=i-index+1;
	_hasBeenWritten[index]=true;
    Base::insert(index, value);
}

const std::vector<int>& Array::data() const
{
	return _data;
}

void Array::fillRandom(const int min, const int max)
{
	const int _max = max - min;
    for (int i=0; i<size(); ++i)
		_data[i] = qrand() % _max + min;
}

void Array::fillSortedRandom(const int min, const int max)
{
	int _max = (max - min) / 2;
	_data[0] = qrand() % _max + min;
    for (int i=1; i<size(); ++i)
	{
		_max = ((max - _data[i-1]) - min) / 2;
		_data[i] = _data[i-1] + qrand() % _max + min;
	}
}


bool Array::hasBeenReadenAt(const uint index)
{
	bool _result = _hasBeenReaden[index];
	_hasBeenReaden[index] = false;
	return _result;
}

bool Array::hasBeenWrittenAt(const uint index)
{
	bool _result = _hasBeenWritten[index];
	_hasBeenWritten[index] = false;
    return _result;
}

int Array::operator[](uint index)
{
    _hasBeenReaden[index] = true;
    _readAccess++;
    return Base::operator[](index);
}

void AbstractArray::operation_sleep(float factor) const
{
    if (wait_for_operations)
        std::this_thread::sleep_for(std::chrono::milliseconds(
            (long)(MainWindow::instruction_duration*factor)
        ));
}
