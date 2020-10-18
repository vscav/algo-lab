#include "lib/tp2.h"
#include <QApplication>
#include <time.h>
#include <math.h>

int isMandelbrot(QVector2D z, int n, QVector2D point){
    // recursiv Mandelbrot
    if(n==0) return false;
    if(sqrt((z.x())*(z.x()) + (z.y())*(z.y())) > 2) return n;
    else
    {
        z.setX(z.x()*z.x() - z.y()*z.y() + point.x());
        z.setY(2*z.x()*z.y() + point.y());
        return isMandelbrot(z, n-1, point);
    }
    return false;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = new MandelbrotWindow(isMandelbrot);
    w->show();

    a.exec();
}



