#include <QApplication>
#include <time.h>

#include "lib/tp1.h"

MainWindow* w=nullptr;


void bubbleSort(Array& toSort){
	// bubbleSort
    int i, j;
        for (i = 0; i < (int) (toSort.size() - 1); i++)
            for (j = 0; j < (int) (toSort.size() - i- 1); j++)
                if (toSort[j] > toSort[j + 1])
                    toSort.swap(j, j + 1);
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    uint elementCount=5;
	MainWindow::instruction_duration = 100;
	w = new TestMainWindow(bubbleSort, elementCount);
	w->show();

	return a.exec();
}
