#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("ZM Music");
    w.setFixedSize(1040,726);
    w.show();
    return a.exec();
}
