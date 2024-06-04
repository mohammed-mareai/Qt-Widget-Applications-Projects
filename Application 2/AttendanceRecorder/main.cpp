#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Icons/icons/icon.png"));
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
