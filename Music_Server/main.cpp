//#include "mainwindow.h"
#include "servercontrol.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerControl w;
    return a.exec();
}//利用MVC模式将主界面与数据处理分开
