#include "dynamicwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DynamicWindow w;
    w.show();
    return a.exec();
}
