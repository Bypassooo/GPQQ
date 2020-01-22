#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    QDir::setCurrent(a.applicationDirPath());//添加这句 立马可以
    w.show();
    return a.exec();
}
