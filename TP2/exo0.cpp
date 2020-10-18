#include <QApplication>
#include <time.h>

#include "lib/tp1.h"

MainWindow* w = nullptr;

void slow_sort(Array& toSort){
    for (int i=0; i<toSort.size(); i++)
    {
        for (int j=0; j<toSort.size(); j++)
        {
            if (toSort[j] > toSort[i])
                toSort.swap(i, j);
        }
    }
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    uint elementCount=5; // number of elements to sort
    MainWindow::instruction_duration = 50; // delay between each array access (set, get, insert, ...)
    w = new TestMainWindow(slow_sort, elementCount); // window which display the behavior of the sort algorithm
    w->show();

    return a.exec();
}
