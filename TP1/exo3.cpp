#include "lib/tp2.h"
#include <QApplication>
#include <time.h>

#define return_and_display(result) return _.store(result);


int search(int value, Array& toSort, int size)
{
    Context _("search", value, size); // do not care about this, it allow the display of call stack

    // your code
    if (size <= 0)
    {
        return - 1;
    } else if (toSort[size - 1] == value)
    {
        return size - 1;
    } else {
        return search(value, toSort, size - 1);
    }

    return_and_display(-1);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // create a window manager
    MainWindow::instruction_duration = 400;  // make a pause between instruction display
    MainWindow* w = new SearchWindow(search); // create a window for this exercice
    w->show(); // show exercice

    return a.exec(); // main loop while window is opened
}




