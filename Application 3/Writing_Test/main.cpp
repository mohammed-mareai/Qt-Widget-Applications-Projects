#include "mainwindow.h"
#include "enter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    enter e;
    e.setWindowIcon(QIcon(":/new/prefix1/icons/enter.jpg"));
    e.show();

    return a.exec();
}
