#include <QApplication>
#include <time.h>

#include "lib/tp1.h"

MainWindow* w = nullptr;

void selectionSort(Array& toSort){
    // selectionSort
    int min = 0;
    for (int i = 0 ; i < (int) toSort.size() ; i++){
        min = i;
        for (int j = i ; j < (int) toSort.size() ; j++) {
            if (toSort[min] > toSort[j]) {
                min = j;
            }
        }
        toSort.swap(i,min);
    }
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    uint elementCount=5; // number of elements to sort
    MainWindow::instruction_duration = 100; // delay between each array access (set, get, insert, ...)
    w = new TestMainWindow(selectionSort, elementCount); // window which display the behavior of the sort algorithm
    w->show();

	return a.exec();
}
