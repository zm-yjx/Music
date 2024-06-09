#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

QPushButton *MainWindow::getcreatbtn()
{
    return this->ui->createbtn;
}

QPushButton *MainWindow::getclosebtn()
{
    return  this->ui->closebtn;
}

QListWidget *MainWindow::getlistwidget()
{
    return this->ui->listWidget;
}

MainWindow::~MainWindow()
{
    delete ui;
}
