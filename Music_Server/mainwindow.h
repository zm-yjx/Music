#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QList>
#include <QListWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    QPushButton *getcreatbtn();
    QPushButton *getclosebtn();
    QListWidget *getlistwidget();
    ~MainWindow();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
