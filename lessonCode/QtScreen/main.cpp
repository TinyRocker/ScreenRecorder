#include "QtScreen.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtScreen w;
    w.show();
    return a.exec();
}
