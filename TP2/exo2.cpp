#include <QApplication>
#include <time.h>

#include "lib/tp1.h"

MainWindow* w=nullptr;

void insertionSort(Array& toSort){
    Array& sorted=w->newArray(toSort.size());
    // insertion sort
    int i, key, j;
    for (i = 1; i < (int) toSort.size(); i++)
    {
        key = toSort[i];
        j = i - 1;
        while (j >= 0 && toSort[j] > key)
        {
            toSort[j + 1] = toSort[j];
            j = j - 1;
        }
        toSort[j + 1] = key;
    }
    toSort=sorted;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uint elementCount=5;
    MainWindow::instruction_duration = 100;
    w = new TestMainWindow(insertionSort, elementCount); // window which display the behavior of the sort algorithm
    w->show();

    return a.exec();
}
