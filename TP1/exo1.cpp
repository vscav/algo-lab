#include "lib/tp2.h"
#include <QApplication>
#include <time.h>

#define return_and_display(result) return _.store(result);

int power(int value, int n)
{
    Context _("power", value, n); // do not care about this, it allow the display of call stack

    // your code
    if(n > 0)
        return (value*power(value, n-1));
    else
        return 1;

    return_and_display(n);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // create a window manager
    MainWindow::instruction_duration = 400;  // make a pause between instruction display
    MainWindow* w = new PowerWindow(power); // create a window for this exercice
    w->show(); // show exercice

    return a.exec(); // main loop while window is opened
}
