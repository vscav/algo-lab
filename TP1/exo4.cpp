#include "lib/tp2.h"
#include <QApplication>
#include <time.h>

#define return_and_display(result) return _.store(result);

void allEvens(Array& evens, Array& array, int evenSize, int arraySize)
{
    Context _("allEvens", evenSize, arraySize); // do not care about this, it allow the display of call stack

    // your code
    if(arraySize <= 0){
        return;
    }
    else if (array[arraySize - 1] % 2 == 0)
    {
        evens[evenSize] = array[arraySize - 1];
        evenSize += 1;
    }
    allEvens(evens, array, evenSize, arraySize - 1);
    return;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // create a window manager
    MainWindow::instruction_duration = 300;  // make a pause between instruction display
    MainWindow* w = new AllEvensWindow(allEvens); // create a window for this exercice
    w->show(); // show exercice

    return a.exec(); // main loop while window is opened
}




