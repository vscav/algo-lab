#include "lib/tp3.h"
#include <QApplication>
#include <time.h>

MainWindow* w = nullptr;
using std::size_t;

void binarySearchAll(Array& array, int toSearch, int& indexMin, int& indexMax)
{
    // Update version

    int beginMin = 0, endMin=(int)array.size()-1, mid;
    indexMin = -1;
    indexMax = -1;

    while(beginMin != endMin)
    {
        mid = (int)(beginMin + endMin)/2;
        if(toSearch < array[mid])
            endMin=mid;
        else if (toSearch > array[mid])
            beginMin = mid+1;
        else
        {
            indexMin = mid;
            endMin = mid;
        }
    }

    if(indexMin != -1)
    {
        int beginMax = indexMin, endMax = (int)array.size()-1;
        while(beginMax != endMax)
        {
            mid = (int)(beginMax + endMax)/2;
            if(array[mid] < toSearch)
                beginMax = mid +1;
            else if (array[mid] == toSearch)
            {
                indexMax = mid;
                beginMax = mid +1;
            }
            else
                endMax = mid;
        }

    }

    // First version

    /*int l = 0;
    int r = array.size() - 1;
    bool bExist = false;
    while (l <= r)
    {
        int m = l + (r - l) / 2;
        if (array.get(m) == toSearch) bExist = true;
        if (array.get(m) < toSearch) l = m + 1;
        else r = m - 1;
    }

    if (bExist)
    {
        indexMin = l;
        int i = indexMin;
        while(array[i] == toSearch)
        {
            i += 1;
            if(i > (int)array.size() - 1) break;
        }
        indexMax = i - 1;
        array.set(indexMin, toSearch);
        array.set(indexMax, toSearch);
    }
    else
    {
        array.set(indexMin, -1);
        array.set(indexMax, -1);
    }*/
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow::instruction_duration = 500;
    w = new BinarySearchAllWindow(binarySearchAll);
    w->show();

    return a.exec();
}
