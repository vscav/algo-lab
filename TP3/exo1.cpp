#include "lib/tp3.h"
#include <QApplication>
#include <time.h>

MainWindow* w = nullptr;
using std::size_t;

int binarySearch(Array& array, int toSearch)
{
    int l = 0;
    int r = array.size() - 1;
    while (l <= r)
    {
        int m = l + (r - l) / 2;
        if (array.get(m) == toSearch) return m;
        if (array.get(m) < toSearch) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow::instruction_duration = 500;
    w = new BinarySearchWindow(binarySearch);
    w->show();

    return a.exec();
}
