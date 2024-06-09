#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QTcpSocket>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QObject>
#include "adduser.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent = nullptr);
    void connectstate();
    QTcpSocket *getSocket();
    static QTcpSocket *getloginsocket();
    ~LoginPage();
public:
    QString username;
    QString userpasswd;
    QString tcptype;
    QString loginstate;
    bool tcpstate=false;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void takenewuserinfo(QString username,QString userpasswd);
private:
    Ui::LoginPage *ui;
    AddUser *adduserpage;
    QTcpSocket *socket=nullptr;   
signals:
    void finishlogin();   
};

#endif // LOGINPAGE_H
