#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <qglobal.h>

using std::size_t;

class AbstractArray {

public:
    virtual ~AbstractArray() {};

public:
    static bool wait_for_operations;
    void operation_sleep(float factor=1.0f) const;
};

template <typename T>
class TemplateArray : public AbstractArray
{
public:
    typedef AbstractArray Base;

    TemplateArray() {}

    TemplateArray(const TemplateArray<T>& other):
        _data(other._data.begin(),other._data.end())
    {}
    TemplateArray(const std::vector<T>& other):
        _data(other.begin(), other.end())
    {}

    virtual ~TemplateArray() {};

    void push_back(int value)
    {
        _data.push_back(value);
    }
    bool empty() const {return _data.empty();}
    int size() const {return _data.size();}

    const T& get(const unsigned int index) const
    {
        if(index >= (int)_data.size())
            throw std::out_of_range("get(): Given index is out of bound !");
        operation_sleep();
        return _data[index];
    }

    T& __get__(const unsigned int index)
    {
        return _data[index];
    }
    const T& __get__(const unsigned int index) const
    {
        return _data[index];
    }

    void set(const int index, const T value)
    {
        if(index >= _data.size())
            throw std::out_of_range("set(): Given index is out of bound !");
        operation_sleep();
        _data[index] = value;
    }

    void __set__(const int index, const T value)
    {
        _data[index] = value;
    }

    void swap(const int index1, const int index2)
    {
        if(index1 >= _data.size() || index2 >= _data.size())
            throw std::out_of_range("set(): Given index is out of bound !");
        const T tmp = _data[index1];
        _data[index1] = _data[index2];
        _data[index2] = tmp;
        operation_sleep(2);
    }
    void insert(const int index, T value)
    {
        uint i=index;
        while(i < _data.size()-1 && _data[i] >= 0)
            i++;
        while (i > index)
        {
            _data[i]=_data[i-1];
            --i;
        }
        _data[index]=value;
    }

    T& operator[](const uint index)
    {
        if(index >= _data.size())
            throw std::out_of_range("get(): Given index is out of bound !");
        operation_sleep();
        return _data[index];
    }

    void operator=(const TemplateArray<T>& other)
    {
        throw std::runtime_error("You should not use the operator = for copying arrays !");
        for (int i=0; i<std::min(size(), other.size()); i++)
        {
            set(i, other[i]);
        }
    }

    const std::vector<T>& data() const {return _data;}

protected:
    std::vector<T> _data;
};

class Array : public TemplateArray<int>
{
public:
    typedef TemplateArray<int> Base;

    Array(uint size=100);
    Array(const Array& other);
    Array(const std::vector<int>& other);

    virtual ~Array() {};

    void resize(uint size);
    void reserve(uint size) {_data.reserve(size);}
    void push_back(int value)
    {
        _data.push_back(value);
        _hasBeenReaden.push_back(false);
        _hasBeenWritten.push_back(true);
    }
    bool empty() const {return _data.empty();}
    uint size() const {return _data.size();}
    uint effectiveSize() const;
    QString toString() const;

    const int& get(const uint index) const;
    void set(const uint index, const int value);
    void swap(const uint index1, const uint index2);

    bool contains(int value) const;

    void fillRandom(const int min = 0, const int max=RAND_MAX);
    void fillSortedRandom(const int min = 0, const int max=RAND_MAX);

    bool hasBeenReadenAt(const uint index);
    bool hasBeenWrittenAt(const uint index);
    uint readAccessCount() const {return _readAccess;}
    uint writeAccessCount() const {return _writeAccess;}

    int operator[](uint index);

    void insert(const uint index, int value);
    const std::vector<int>& data() const;
private:
    std::vector<bool> _hasBeenReaden;
    std::vector<bool> _hasBeenWritten;
    uint _readAccess;
    uint _writeAccess;
};

#endif // ARRAY_H
