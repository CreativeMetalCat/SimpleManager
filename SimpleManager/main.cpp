#include "SimpleManager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleManager w;
    w.show();
    return a.exec();
}
