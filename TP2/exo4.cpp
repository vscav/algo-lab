#include <QApplication>
#include <time.h>

#include "lib/tp1.h"


MainWindow* w=nullptr;

void recursivQuickSort(Array& toSort, int size)
{
    if (size <= 1) {
        return;
    }

    // stop statement = condition + return (return stop the function even if it does not return anything)
    Array& lowerArray = w->newArray(size);
    Array& greaterArray= w->newArray(size);
    int lowerSize = 0, greaterSize = 0;
    int pivot = toSort[0];

    // split
    for (int i = 1; i < size; i++){
        if (pivot > toSort[i]) {
            lowerArray[lowerSize] = toSort[i];
            lowerSize++;
        }
        else {
            greaterArray[greaterSize] = toSort[i];
            greaterSize++;
        }
    }

    // recursiv sort of lowerArray and greaterArray
    recursivQuickSort(lowerArray,lowerSize);
    recursivQuickSort(greaterArray,greaterSize);

    // merge
    int j = 0;
    for (int i = 0 ; i < lowerSize ; i++) {
        toSort[i] = lowerArray[i];

    }
    toSort[lowerSize] = pivot;
    for (int i = lowerSize + 1  ; i < lowerSize + greaterSize + 1 ; i++) {
        toSort[i] = greaterArray[j];
        j++;
    }
}

void quickSort(Array& toSort){
    recursivQuickSort(toSort, toSort.size());
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uint elementCount=8;
    MainWindow::instruction_duration = 50;
    w = new TestMainWindow(quickSort, elementCount);
    w->show();

    return a.exec();
}
