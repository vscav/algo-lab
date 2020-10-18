#include <QApplication>
#include <time.h>

#include "lib/tp1.h"

MainWindow* w=nullptr;

void merge(Array& first, Array& second, Array& result);

void splitAndMerge(Array& origin)
{
    // stop statement = condition + return (return stop the function even if it does not return anything)
    if (origin.size() <= 1){
        return;
    }

    Array& first = w->newArray(origin.size()/2);
    Array& second = w->newArray(origin.size()-first.size());

    // split
    for (int i = 0 ; i < (int)first.size() ; i++){
        first[i] = origin[i];
    }
    for (int i = 0 ; i < (int)second.size() ; i++){
        second[i] = origin[i + first.size()];
    }

    // recursiv splitAndMerge of lowerArray and greaterArray
    splitAndMerge(first);
    splitAndMerge(second);

    // merge
    merge(first,second,origin);
}

void merge(Array& first, Array& second, Array& result)
{
    // Sorted merge -> merge by checking for all elements which should be stored first into result
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < (int)first.size() && j < (int)second.size()){
        if (first[i] < second[j]){
            result[k] = first[i];
            i++;
            k++;
        }
        else {
            result[k] = second[j];
            j++;
            k++;
        }
    }
    while ( i < (int)first.size()) {
        result[k] = first[i];
        i++;
        k++;
    }
    while ( j < (int)second.size()){
        result[k] = second[j];
        j++;
        k++;
    }
}

void mergeSort(Array& toSort)
{
    splitAndMerge(toSort);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uint elementCount=20;
    MainWindow::instruction_duration = 10;
    w = new TestMainWindow(mergeSort, elementCount);
    w->show();

    return a.exec();
}
